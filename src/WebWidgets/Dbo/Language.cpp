#include "Dbo/Language.h"

//Language Single Key
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

//Language Plural Key
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
