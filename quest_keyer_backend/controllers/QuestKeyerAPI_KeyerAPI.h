#pragma once

#include <drogon/HttpController.h>
#include <drogon/LocalHostFilter.h>

#include "quest_keyer/keyer/quest_keyer.h"

using namespace drogon;

namespace QuestKeyerAPI
{
class KeyerAPI : public drogon::HttpController<KeyerAPI>
{
    std::string config_path = "../quest_keyer/keyerconfig.json";
    Quest::KeyerConfig keyer_config;
    Quest::Keyer seq_keyer;

  public:
    METHOD_LIST_BEGIN

    METHOD_ADD(KeyerAPI::Init, "/init", Get);
    METHOD_ADD(KeyerAPI::ChromaKey, "/chromakey?keyr={1}&keyg={2}&keyb={3}&threshold={4}", Get);

    METHOD_LIST_END

    void Init(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback);
    void ChromaKey(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback,
        const std::string &key_r, const std::string &key_g, const std::string &key_b, const std::string &threshold);
};
}
