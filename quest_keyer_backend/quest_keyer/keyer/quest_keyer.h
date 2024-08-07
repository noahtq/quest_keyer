//
// Created by Noah Turnquist on 8/2/24.
//

#ifndef QUEST_KEYER_H
#define QUEST_KEYER_H

#include <opencv2/opencv.hpp>
#include "../include/json.hpp"
#include "../include/quest_seq_lib.h"

namespace Quest {
    struct KeyerConfig {
        std::string temp_dir;
        std::string proxy_path;
        void populate(const std::string& config_path);
    };

    class Keyer {
        bool keyed = false;
    public:
        // Public member variables
        ImageSeq image_seq;

        // Getters and setters

        // Member functions
        void chromaKey(const cv::Scalar& key_value, const int& threshold);
    };
}

#endif //QUEST_KEYER_H
