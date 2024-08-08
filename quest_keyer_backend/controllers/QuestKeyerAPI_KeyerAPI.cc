#include <fstream>
#include "QuestKeyerAPI_KeyerAPI.h"
#include "../include/json.hpp"

using namespace QuestKeyerAPI;
using json = nlohmann::json;

void KeyerConfig::populate(const std::filesystem::path& config_path) {
    std::ifstream ifs(config_path);
    json config_params = json::parse(ifs);
    temp_path = std::filesystem::path(config_params.at("temp-dir-path"));
    proxy_path = std::filesystem::path(config_params.at("proxy-dir-path"));
}


void KeyerAPI::Init(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    LOG_DEBUG << "Initializing Program";

    keyer_config.populate(config_path);

    LOG_DEBUG << "Loaded keyer config file";

    if (std::filesystem::is_directory(keyer_config.temp_path)) {
        LOG_DEBUG << "Temp dir already exists, deleting contents";
        for (const auto& entry : std::filesystem::directory_iterator(keyer_config.temp_path)) {
            std::filesystem::remove_all(entry.path());
        }
    } else {
        std::filesystem::create_directory(keyer_config.temp_path);
        LOG_DEBUG << "Created temp directory";
    }

    std::filesystem::create_directory(keyer_config.temp_path / keyer_config.proxy_path);

    Json::Value ret;
    ret["result"] = "ok";
    ret["message"] = "Finished backend init";
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);

    initialized = true;
}

void KeyerAPI::OpenSeq(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string& input_path) {
    assert(initialized);
    LOG_DEBUG << "Opening Image Sequence";

    Quest::SeqErrorCodes code = seq_keyer.image_seq.open(std::filesystem::path(input_path));

    Json::Value ret;
    switch(code) {
    case Quest::SeqErrorCodes::Success: {
        LOG_DEBUG << "Succesfully opened image sequence";

        //TODO: probably turn all of this into separate function since it is being repeated x2
        std::filesystem::path proxy_dir = keyer_config.temp_path / keyer_config.proxy_path / std::to_string(proxy_id);
        std::filesystem::create_directory(proxy_dir);

        std::filesystem::path proxy_path =
            proxy_dir / seq_keyer.image_seq.get_input_path().filename();
        proxy_path.replace_extension(".jpg");

        Quest::Proxy proxy_seq(seq_keyer.image_seq, 0.25);
        code = proxy_seq.render(proxy_path);
        ++proxy_id;

        std::filesystem::path keyer_proxy_dir = keyer_config.temp_path / keyer_config.proxy_path / std::to_string(proxy_id);
        std::filesystem::create_directory(keyer_proxy_dir);

        std::filesystem::path keyer_proxy_path =
            keyer_proxy_dir / seq_keyer.image_seq.get_input_path().filename();
        keyer_proxy_path.replace_extension(".jpg");
        keyer_proxy = new Quest::Proxy(seq_keyer.image_seq, 0.25);
        Quest::SeqErrorCodes code_2 = keyer_proxy->render((keyer_proxy_path));

        if (code == Quest::SeqErrorCodes::Success && code_2 == Quest::SeqErrorCodes::Success) {
            LOG_DEBUG << "Created proxy sequence at " << proxy_path.string();

            ret["result"] = "ok";
            ret["message"] = "Successfully opened image sequence";
            ret["orig-proxy-path"] = proxy_path.string();
            ret["keyer-proxy-path"] = keyer_proxy_path.string();
            ret["frame-count"] = seq_keyer.image_seq.get_frame_count();
        } else {
            LOG_DEBUG << "Error: " << "Proxy sequence couldn't be created.";

            ret["result"] = "fail";
            ret["message"] = "There was an error creating the proxy sequence";
        }
        break;
    }
    case Quest::SeqErrorCodes::BadPath:
        LOG_DEBUG << "Error: Bad Image Sequence Path";

        ret["result"] = "fail";
        ret["message"] = "Couldn't find an image sequence at the given path";
        break;
    case Quest::SeqErrorCodes::UnsupportedExtension:
        LOG_DEBUG << "Error: Unsupported extensions";

        ret["result"] = "fail";
        ret["message"] = "Attempting to open an unsupported image type";
        break;
    default:
        LOG_DEBUG << "Error: Unknown error";

        ret["result"] = "fail";
        ret["message"] = "An unknown error occured";
    }

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
