#ifndef WW_DBO_MODULE_H
#define WW_DBO_MODULE_H

#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	namespace Dbo
	{
		class BaseModule
		{
		public:
			std::string name;
			int versionSeries = -1;
			int versionMajor = -1;
			int versionMinor = -1;

		protected:
			BaseModule() = default;
			BaseModule(long long id)
				: _id(id)
			{ }

			long long _id = -1;
		};

		class Module : public BaseModule
		{
		public:
			ConfigurationCollection configurationCollection;
			PageCollection pageCollection;
			TemplateCollection templateCollection;
			SingularKeyCollection singularKeyCollection;
			PluralKeyCollection pluralKeyCollection;

			ptr<Author> authorPtr;

			Module() = default;
			Module(long long id)
				: BaseModule(id)
			{ }

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::id(a, _id);
				Wt::Dbo::field(a, name, "name", 255);
				Wt::Dbo::field(a, versionSeries, "versionSeries");
				Wt::Dbo::field(a, versionMajor, "versionMajor");
				Wt::Dbo::field(a, versionMinor, "versionMinor");

				Wt::Dbo::belongsTo(a, authorPtr, "author", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::hasMany(a, configurationCollection, Wt::Dbo::ManyToOne, "module");
				Wt::Dbo::hasMany(a, pageCollection, Wt::Dbo::ManyToOne, "module");
				Wt::Dbo::hasMany(a, templateCollection, Wt::Dbo::ManyToOne, "module");
				Wt::Dbo::hasMany(a, singularKeyCollection, Wt::Dbo::ManyToOne, "module");
				Wt::Dbo::hasMany(a, pluralKeyCollection, Wt::Dbo::ManyToOne, "module");
			}
			constexpr static const char *tableName()
			{
				return "module";
			}
		};
	}

	namespace Ddo
	{
		class Module : public Dbo::BaseModule
		{
		public:
			long long authorId;

			Module(Dbo::ptr<Dbo::Module> ptr)
				: BaseModule(*ptr), authorId(ptr->authorPtr.id())
			{ }

			long long id() const
			{
				return _id;
			}
		};
	}
}

#endif
