//
// Created by Noah Turnquist on 8/2/24.
//

#ifndef QUEST_KEYER_H
#define QUEST_KEYER_H

#include <opencv2/opencv.hpp>
#include "../../include/quest_seq_lib.h"

namespace Quest {
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
