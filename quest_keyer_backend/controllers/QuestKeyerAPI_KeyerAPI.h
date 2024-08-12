#pragma once

#include <drogon/HttpController.h>
#include <drogon/LocalHostFilter.h>

#include "quest_keyer/keyer/quest_keyer.h"
#include "quest_keyer/include/quest_seq_lib.h"

using namespace drogon;

namespace QuestKeyerAPI
{
    struct KeyerConfig {
        std::filesystem::path temp_path;
        std::filesystem::path proxy_path;
        double proxy_size = 0.5;
        void populate(const std::filesystem::path& config_path);
    };

class KeyerAPI : public drogon::HttpController<KeyerAPI>
{
    std::filesystem::path config_path = "../keyerconfig.json";
    KeyerConfig keyer_config;
    bool initialized = false;
    Quest::ImageSeq keyer_seq;
    Quest::Proxy *orig_proxy = nullptr;
    Quest::Proxy *keyer_proxy = nullptr;
    int proxy_id = 0;

  public:
    METHOD_LIST_BEGIN

    METHOD_ADD(KeyerAPI::Init, "/init", Get);
    METHOD_ADD(KeyerAPI::OpenSeq, "/open?path={1}", Get);
    METHOD_ADD(KeyerAPI::ChromaKey, "/chromakey?keyr={1}&keyg={2}&keyb={3}&threshold={4}", Get);
    METHOD_ADD(KeyerAPI::ExportSeq, "/export?path={1}&keyr={2}&keyg={3}&keyb={4}&threshold={5}", Get);

    METHOD_LIST_END

    // API Endpoint Handlers
    void Init(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback);
    void OpenSeq(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback,
        const std::string &input_path);
    void ChromaKey(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback,
        const std::string &key_r, const std::string &key_g, const std::string &key_b, const std::string &threshold) const;
    void ExportSeq(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback,
        const std::string &output_path,
        const std::string &key_r, const std::string &key_g, const std::string &key_b, const std::string &threshold);

    // Destructor
    ~KeyerAPI();
};
    // Helper functions
    bool VerifyKeyValues(const std::string &key_r, const std::string &key_g, const std::string &key_b, const std::string &threshold);
}
