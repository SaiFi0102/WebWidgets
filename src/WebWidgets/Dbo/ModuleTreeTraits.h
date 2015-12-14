#ifndef WW_DBO_DBOTRAITS_H
#define WW_DBO_DBOTRAITS_H

//Header file requires Dbos to be forward declared

namespace WW
{
	namespace Ddo
	{
		class SurrogateKey
		{
		public:
			long long id() const { return _id; }

		protected:
			SurrogateKey(long long id)
				: _id(id)
			{ };

			long long _id;
		};
	}
}

namespace Wt
{
	namespace Dbo
	{
		template<>
		struct dbo_traits<WW::Dbo::Author> : public dbo_default_traits
		{
			constexpr static const char *surrogateIdField() { return nullptr; }
		};
		template<>
		struct dbo_traits<WW::Dbo::Module> : public dbo_default_traits
		{
			constexpr static const char *surrogateIdField() { return nullptr; }
		};

		//ConfigurationBool
		template<>
		struct dbo_traits<WW::Dbo::ConfigurationBool> : public dbo_default_traits
		{
			typedef WW::Dbo::ptr<WW::Dbo::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			constexpr static const char *surrogateIdField() { return nullptr; }
		};
		//ConfigurationDouble
		template<>
		struct dbo_traits<WW::Dbo::ConfigurationDouble> : public dbo_default_traits
		{
			typedef WW::Dbo::ptr<WW::Dbo::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			constexpr static const char *surrogateIdField() { return nullptr; }
		};
		//ConfigurationEnum
		template<>
		struct dbo_traits<WW::Dbo::ConfigurationEnum> : public dbo_default_traits
		{
			typedef WW::Dbo::ptr<WW::Dbo::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			constexpr static const char *surrogateIdField() { return nullptr; }
		};
		//ConfigurationFloat
		template<>
		struct dbo_traits<WW::Dbo::ConfigurationFloat> : public dbo_default_traits
		{
			typedef WW::Dbo::ptr<WW::Dbo::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			constexpr static const char *surrogateIdField() { return nullptr; }
		};
		//ConfigurationInt
		template<>
		struct dbo_traits<WW::Dbo::ConfigurationInt> : public dbo_default_traits
		{
			typedef WW::Dbo::ptr<WW::Dbo::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			constexpr static const char *surrogateIdField() { return nullptr; }
		};
		//ConfigurationLongInt
		template<>
		struct dbo_traits<WW::Dbo::ConfigurationLongInt> : public dbo_default_traits
		{
			typedef WW::Dbo::ptr<WW::Dbo::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			constexpr static const char *surrogateIdField() { return nullptr; }
		};
		//ConfigurationString
		template<>
		struct dbo_traits<WW::Dbo::ConfigurationString> : public dbo_default_traits
		{
			typedef WW::Dbo::ptr<WW::Dbo::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			constexpr static const char *surrogateIdField() { return nullptr; }
		};

		//Language
		template<>
		struct dbo_traits<WW::Dbo::Language> : public dbo_default_traits
		{
			typedef std::string IdType;
			static IdType invalidId() { return IdType(); }
			constexpr static const char *surrogateIdField() { return nullptr; }
		};

		//AccessHostName
		template<>
		struct dbo_traits<WW::Dbo::AccessHostName> : public dbo_default_traits
		{
			typedef std::string IdType;
			static IdType invalidId() { return "!"; }
			constexpr static const char *surrogateIdField() { return nullptr; }
		};

		//MenuItemPage custom ManyToMany
		template <class Action>
		void field(Action &action, WW::Dbo::MenuItemOnPageKey &key, const std::string &, int size = -1)
		{
			Wt::Dbo::belongsTo(action, key.menuItemPtr, "navigationMenuItem", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::belongsTo(action, key.pagePtr, "page", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		template<>
		struct dbo_traits<WW::Dbo::MenuItemOnPage> : public dbo_default_traits
		{
			typedef WW::Dbo::MenuItemOnPageKey IdType;
			static IdType invalidId();
			constexpr static const char *surrogateIdField() { return nullptr; }
		};

		//MenuItemPageAccessPath custom ManyToMany
		template <class Action>
		void field(Action &action, WW::Dbo::MenuItemOnPageAccessPathKey &key, const std::string &, int size = -1)
		{
			Wt::Dbo::belongsTo(action, key.menuItemPtr, "navigationMenuItem", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::belongsTo(action, key.accessPathPtr, "pageAccessPath", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		template<>
		struct dbo_traits<WW::Dbo::MenuItemOnPageAccessPath> : public dbo_default_traits
		{
			typedef WW::Dbo::MenuItemOnPageAccessPathKey IdType;
			static IdType invalidId();
			constexpr static const char *surrogateIdField() { return nullptr; }
		};
	}
}

#endif
