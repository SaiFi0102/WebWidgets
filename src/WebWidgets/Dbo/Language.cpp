#include "Dbo/Language.h"

//Language Single Keys
LanguageSingleKeys::LanguageSingleKeys()
{ }
LanguageSingleKeys::LanguageSingleKeys(Wt::Dbo::ptr<Language> LanguagePtr, const std::string &Key, Wt::Dbo::ptr<Module> ModulePtr)
	: LanguagePtr(LanguagePtr), Key(Key), ModulePtr(ModulePtr)
{ }
bool LanguageSingleKeys::operator== (const LanguageSingleKeys &other) const
{
	return LanguagePtr == other.LanguagePtr && Key == other.Key && ModulePtr == other.ModulePtr;
}
bool LanguageSingleKeys::operator< (const LanguageSingleKeys &other) const
{
	if(LanguagePtr < other.LanguagePtr)
	{
		return true;
	}
	else if(LanguagePtr == other.LanguagePtr)
	{
		if(Key < other.Key)
		{
			return true;
		}
		else if(Key == other.Key)
		{
			return ModulePtr < other.ModulePtr;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
std::ostream &operator<< (std::ostream &o, const LanguageSingleKeys &c)
{
	return o << "(" << c.LanguagePtr << ", " << c.Key << ", " << c.ModulePtr << ")";
}

//Language Plural Keys
LanguagePluralKeys::LanguagePluralKeys()
	: Case(-1)
{ }
LanguagePluralKeys::LanguagePluralKeys(Wt::Dbo::ptr<Language> LanguagePtr, const std::string &Key, Wt::Dbo::ptr<Module> ModulePtr, int Case)
	: LanguageSingleKeys(LanguagePtr, Key, ModulePtr), Case(Case)
{ }
bool LanguagePluralKeys::operator== (const LanguagePluralKeys &other) const
{
	return LanguagePtr == other.LanguagePtr && Key == other.Key && Case == other.Case && ModulePtr == other.ModulePtr;
}
bool LanguagePluralKeys::operator< (const LanguagePluralKeys &other) const
{
	if(LanguagePtr < other.LanguagePtr)
	{
		return true;
	}
	else if(LanguagePtr == other.LanguagePtr)
	{
		if(Key < other.Key)
		{
			return true;
		}
		else if(Key == other.Key)
		{
			if(ModulePtr < other.ModulePtr)
			{
				return true;
			}
			else if(ModulePtr == other.ModulePtr)
			{
				return Case < other.Case;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
std::ostream &operator<< (std::ostream &o, const LanguagePluralKeys &c)
{
	return o << "(" << c.LanguagePtr << ", " << c.Key << ", " << c.ModulePtr << ", " << c.Case << ")";
}

//Overloaded dbo_trait for Language DBO
Wt::Dbo::dbo_traits<Language>::IdType Wt::Dbo::dbo_traits<Language>::invalidId()
{
	return Wt::Dbo::dbo_traits<Language>::IdType();
}
const char* Wt::Dbo::dbo_traits<Language>::surrogateIdField()
{
	return 0;
}

//Overloaded dbo_trait for LanguageSingle DBO
Wt::Dbo::dbo_traits<LanguageSingle>::IdType Wt::Dbo::dbo_traits<LanguageSingle>::invalidId()
{
	return Wt::Dbo::dbo_traits<LanguageSingle>::IdType();
}
const char* Wt::Dbo::dbo_traits<LanguageSingle>::surrogateIdField()
{
	return 0;
}

//Overloaded dbo_trait for LanguagePlural DBO
Wt::Dbo::dbo_traits<LanguagePlural>::IdType Wt::Dbo::dbo_traits<LanguagePlural>::invalidId()
{
	return Wt::Dbo::dbo_traits<LanguagePlural>::IdType();
}
const char* Wt::Dbo::dbo_traits<LanguagePlural>::surrogateIdField()
{
	return 0;
}

Language::Language()
	: PluralCases(-1)
{ }

LanguageSingle::LanguageSingle()
	: IsEmail(false)
{ }
LanguageSingle::LanguageSingle(LanguageSingleKeys Key)
	: _Id(Key), IsEmail(false)
{ }
LanguageSingle::LanguageSingle(const std::string &Key, const std::string &String, Wt::Dbo::ptr<Module> ModulePtr, Wt::Dbo::ptr<Language> LanguagePtr, bool IsEmail)
	: _Id(LanguagePtr, Key, ModulePtr), String(String), IsEmail(IsEmail)
{ }

LanguagePlural::LanguagePlural()
{ }
LanguagePlural::LanguagePlural(LanguagePluralKeys Key)
	: _Id(Key)
{ }