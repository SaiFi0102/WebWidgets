#ifndef WW_DBOLOCALIZEDSTRINGS_H
#define WW_DBOLOCALIZEDSTRINGS_H

#include <Wt/WLocalizedStrings>

namespace WW
{
	class WServer;

	class DboLocalizedStrings : public Wt::WLocalizedStrings
	{
	public:
		DboLocalizedStrings(WServer *server);

		virtual bool resolveKey(const std::string &key, std::string &result) override;
		virtual bool resolveKey(const std::string &key, long long moduleId, std::string &result) override;
		virtual bool resolvePluralKey(const std::string &key, std::string &result, uint64_t amount) override;
		virtual bool resolvePluralKey(const std::string &key, long long moduleId, std::string &result, uint64_t amount) override;

		virtual bool resolveTemplateKey(const std::string &templateName, long long moduleId, std::string &result) override;
		virtual bool loadTemplateStyleSheet(const std::string &templateName, long long moduleId) override;

	protected:
		WServer *_server;
	};
}

#endif