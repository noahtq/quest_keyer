//
// Created by Noah Turnquist on 8/2/24.
//

#include "quest_keyer.h"

void Quest::KeyerConfig::populate(const std::string& config_path) {
    nlohmann::json config_params = nlohmann::json::parse(std::filesystem::path(config_path));
    temp_dir = config_params.at("temp-dir-path");
    proxy_path = config_params.at("proxy-dir-path");
}

void Quest::Keyer::chromaKey(const cv::Scalar& key_value, const int& threshold) {
    cv::Scalar low_bound, high_bound;
    for (int i = 0; i < 3; i++) {
        low_bound[i] = key_value[i] - threshold;
        high_bound[i] = key_value[i] + threshold;
    }

    for (cv::Mat& frame : image_seq) {
        cv::Mat mask, result_1, result_2;
        cv::inRange(frame, cv::Scalar(0, 0, 0), low_bound, mask);
        frame.copyTo(result_1, mask);
        cv::inRange(frame, high_bound, cv::Scalar(255, 255, 255), mask);
        frame.copyTo(result_2, mask);
        frame = result_1 + result_2;
    }
}
