/*
 * Copyright (C) 2009 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/test/unit_test.hpp>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Postgres>
#include <Wt/Dbo/backend/MySQL>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/backend/Firebird>
#include <Wt/WDate>
#include <Wt/WDateTime>
#include <Wt/WTime>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Dbo/ptr_tuple>

#include <string>
namespace dbo = Wt::Dbo;

class TestDbo;
typedef dbo::collection< dbo::ptr<TestDbo> > TestDboCollection;
class TestDbo : public Wt::Dbo::Dbo<TestDbo>
{
public:
        int intC;
        double doubleC;

        template<class Action>
        void persist(Action &a)
        {
                Wt::Dbo::field(a, intC, "intC");
                Wt::Dbo::field(a, doubleC, "doubleC");
        }
        static const char *TableName()
        {
                return "func";
        }
};


struct Dbo3Fixture
{
	typedef std::map<long long, Wt::Dbo::ptr<TestDbo>> TestMapType;
	TestMapType TestMap;
	dbo::SqlConnection *connection_;
	dbo::Session *session_;

  Dbo3Fixture()
  {
    connection_ = new dbo::backend::Sqlite3(":memory:");
    connection_->setProperty("show-queries", "true");

    session_ = new dbo::Session();
    session_->setConnection(*connection_);
    session_->mapClass<TestDbo>(TestDbo::TableName());
    session_->createTables();

	{
		dbo::Transaction tr(*session_);
		dbo::ptr<TestDbo> New1 = session_->add(new TestDbo());
		New1.modify()->intC = 1;
		New1.modify()->doubleC = 1.1;
		dbo::ptr<TestDbo> New2 = session_->add(new TestDbo());
		New2.modify()->intC = 2;
		New2.modify()->doubleC = 2.2;
	}

	{
		//Check initial values
		FetchAll();
		dbo::ptr<TestDbo> DisconnectedTestPtr = TestMap[1]; //id: 1
		std::cout << "Before modified: intC: " << DisconnectedTestPtr->intC << ", expected: 1" << std::endl;

		//Modify from outside session_'s knowledge
		dbo::Transaction tr(*session_);
		session_->execute(std::string("UPDATE ") + TestDbo::TableName() + " SET intC = 99 WHERE id = 1;");
		tr.commit();

		//Check again
		FetchAll();
		//should remain unchanged because in FetchAll() none of the ptr<C>s are deleted or modified but swapped with a local TestMap object
		std::cout << "After re-fetched: intC: " << DisconnectedTestPtr->intC << ", expected: 1" << std::endl;
		//should be a new value because FetchAll uses a collection to map all ptr<C> with newer values from the database
		dbo::ptr<TestDbo> NormalTestPtr = TestMap[1]; //id: 1
		std::cout << "After re-assignment: intC: " << NormalTestPtr->intC << ", expected: 99" << std::endl;
		BOOST_REQUIRE(DisconnectedTestPtr->intC == 1);
		BOOST_REQUIRE(NormalTestPtr->intC == 99);

		dbo::Transaction tr2(*session_);
		NormalTestPtr.modify()->intC = 10;
		tr2.commit();

		try
		{
			dbo::Transaction tr3(*session_);
			DisconnectedTestPtr.modify()->intC = 5;
			tr3.commit();
		}
		catch(std::exception &e)
		{
			std::cout << "Exception caught: " << e.what();
		}
	}
  }

  void FetchAll()
  {
	  //Strong exception safety(make TestMap empty by swapping with an empty map, swap back if exception is caught)
	  TestMapType testmap;
	  TestMap.swap(testmap);

	  session_->disconnectAll();

	  try
	  {
		  dbo::Transaction tr(*session_);
		  TestDboCollection collection = session_->find<TestDbo>();
		  for(TestDboCollection::const_iterator itr = collection.begin();
			  itr != collection.end();
			  ++itr)
		  {
			  TestMap[itr->id()] = *itr;
		  }
		  tr.commit();
	  }
	  catch(...)
	  {
		  testmap.swap(TestMap);
	  }
  }

  ~Dbo3Fixture()
  {
    session_->dropTables();

    delete session_;
    delete connection_;
  }
};

BOOST_AUTO_TEST_CASE( dbo3_reload_test )
{
	Dbo3Fixture f;
}