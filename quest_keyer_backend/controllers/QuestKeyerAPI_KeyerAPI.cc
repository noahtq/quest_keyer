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
    proxy_size = config_params.at("proxy-size");
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

    Quest::SeqErrorCodes code = keyer_seq.open(std::filesystem::path(input_path));

    Json::Value ret;
    switch(code) {
    case Quest::SeqErrorCodes::Success: {
        LOG_DEBUG << "Succesfully opened image sequence";

        // Deallocate prior proxy object in case this function is run more than once, to avoid memory leaks
        if (orig_proxy != nullptr) {
            LOG_DEBUG << "Deallocating Proxy Object";
            delete orig_proxy;
            orig_proxy = nullptr;
        }
        if (keyer_proxy != nullptr) {
            LOG_DEBUG << "Deallocating Proxy Object";
            delete keyer_proxy;
            keyer_proxy = nullptr;
        }

        std::filesystem::path proxy_dir = keyer_config.temp_path / keyer_config.proxy_path / std::to_string(proxy_id);
        std::filesystem::create_directory(proxy_dir);

        std::filesystem::path proxy_path =
            proxy_dir / keyer_seq.get_input_path().filename();
        proxy_path.replace_extension(".jpg");

        orig_proxy = new Quest::Proxy(keyer_seq, keyer_config.proxy_size);
        code = orig_proxy->render(proxy_path);
        ++proxy_id;

        std::filesystem::path keyer_proxy_dir = keyer_config.temp_path / keyer_config.proxy_path / std::to_string(proxy_id);
        std::filesystem::create_directory(keyer_proxy_dir);

        std::filesystem::path keyer_proxy_path =
            keyer_proxy_dir / keyer_seq.get_input_path().filename();
        keyer_proxy_path.replace_extension(".jpg");

        keyer_proxy = new Quest::Proxy(keyer_seq, keyer_config.proxy_size);
        Quest::SeqErrorCodes code_2 = keyer_proxy->render((keyer_proxy_path));

        if (code == Quest::SeqErrorCodes::Success && code_2 == Quest::SeqErrorCodes::Success) {
            ret["result"] = "ok";
            ret["message"] = "Successfully opened image sequence";
            ret["orig-proxy-path"] = (std::filesystem::canonical(proxy_path.parent_path()) / proxy_path.filename()).string();
            ret["keyer-proxy-path"] = (std::filesystem::canonical(keyer_proxy_path.parent_path()) / keyer_proxy_path.filename()).string();
            ret["frame-count"] = keyer_seq.get_frame_count();
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
    const std::string &key_r, const std::string &key_g, const std::string &key_b, const std::string &threshold) const{
    LOG_DEBUG << "Received Key Parameters from Client";
    assert(keyer_seq.get_frame_count() > 0);

    Json::Value ret;
    try {
        const int r_val = std::stoi(key_r);
        const int g_val = std::stoi(key_g);
        const int b_val = std::stoi(key_b);
        const int threshold_val = std::stoi(threshold);

        if ((r_val >= 0 && r_val <= 255) &&
            (g_val >= 0 && g_val <= 255) &&
            (b_val >= 0 && b_val <= 255) &&
            (threshold_val >= 0 && threshold_val <= 255)) {

            Quest::ChromaKey(*orig_proxy, *keyer_proxy, cv::Scalar(r_val, g_val, b_val), threshold_val);
            keyer_proxy->render(keyer_proxy->get_output_path());

            LOG_DEBUG << "Successfully keyed image";
            ret["result"] = "ok";
            ret["message"] = "Successfully keyed image sequence";
            ret["keyer-proxy-path"] = (std::filesystem::canonical(keyer_proxy->get_output_path().parent_path()) / keyer_proxy->get_output_path().filename()).string();
        } else {
            ret["result"] = "fail";
            ret["message"] = "All of the key parameters must be between 0 and 255";
        }
    } catch(std::invalid_argument& e) {
        ret["result"] = "fail";
        ret["message"] = "All of the key parameters must be integers";
    }
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

KeyerAPI::~KeyerAPI() {
    delete orig_proxy;
    delete keyer_proxy;
}

