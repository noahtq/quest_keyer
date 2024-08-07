#include "QuestKeyerAPI_KeyerAPI.h"

using namespace QuestKeyerAPI;

void KeyerAPI::Init(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    LOG_DEBUG << "Initializing Program";

    keyer_config.populate(config_path);

    LOG_DEBUG << "Loading keyer config file";

    Json::Value ret;
    ret["result"] = "ok";
    ret["message"] = "Finished backend init";
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void KeyerAPI::ChromaKey(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback,
    const std::string &key_r, const std::string &key_g, const std::string &key_b, const std::string &threshold) {
    LOG_DEBUG << "Getting Key Parameters from Client";

    Json::Value ret;
    ret["key_r"] = key_r;
    ret["key_g"] = key_g;
    ret["key_b"] = key_b;
    ret["threshold"] = threshold;
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
