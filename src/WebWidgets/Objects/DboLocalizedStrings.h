#ifndef DBOLOCALIZEDSTRINGS_H
#define DBOLOCALIZEDSTRINGS_H

#include <Wt/WLocalizedStrings>

class WServer;

class DboLocalizedStrings : public Wt::WLocalizedStrings
{
	public:
		DboLocalizedStrings(WServer *Server);

		virtual bool resolveKey(const std::string &key, std::string &result);
		virtual bool resolveKey(const std::string &key, long long ModuleId, std::string &result);
		virtual bool resolvePluralKey(const std::string &key, std::string &result, uint64_t amount);
		virtual bool resolvePluralKey(const std::string &key, long long ModuleId, std::string &result, uint64_t amount);

		virtual bool resolveTemplateKey(const std::string &templateName, long long moduleId, std::string &result);
		virtual bool loadTemplateStyleSheet(const std::string &templateName, long long moduleId);

	protected:
		WServer *_Server;
};

#endif