#ifndef DBOLOCALIZEDSTRINGS_H
#define DBOLOCALIZEDSTRINGS_H

#include <Wt/WLocalizedStrings>

class LanguagesDatabase;

class DboLocalizedStrings : public Wt::WLocalizedStrings
{
	public:
		DboLocalizedStrings(LanguagesDatabase *Languages);

		virtual void refresh();
		virtual void hibernate();

		virtual bool resolveKey(const std::string &key, std::string &result);
		virtual bool resolveKey(const std::string &key, long long ModuleId, std::string &result);
		virtual bool resolvePluralKey(const std::string &key, std::string &result, uint64_t amount);
		virtual bool resolvePluralKey(const std::string &key, long long ModuleId, std::string &result, uint64_t amount);

	protected:
		LanguagesDatabase *Languages;
};

#endif