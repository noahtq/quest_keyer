//
// Created by Noah Turnquist on 8/2/24.
//

#ifndef QUEST_KEYER_H
#define QUEST_KEYER_H

#include <opencv2/opencv.hpp>
#include "../include/quest_seq_lib.h"

namespace Quest {
    void ChromaKey(const ImageSeq& original_seq, ImageSeq& destination_seq, const cv::Scalar& key_value, const int& threshold);
}

#endif //QUEST_KEYER_H
