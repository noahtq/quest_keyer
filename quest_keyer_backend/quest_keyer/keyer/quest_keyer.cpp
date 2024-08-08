//
// Created by Noah Turnquist on 8/2/24.
//

#include "quest_keyer.h"

void Quest::ChromaKey(const ImageSeq& original_seq, ImageSeq& destination_seq, const cv::Scalar& key_value, const int& threshold) {
    cv::Scalar low_bound, high_bound;
    for (int i = 0; i < 3; i++) {
        low_bound[i] = key_value[i] - threshold;
        high_bound[i] = key_value[i] + threshold;
    }

    for (int i = 0; i < original_seq.get_frame_count(); i++) {
        cv::Mat frame;
        original_seq.get_frame(i).copyTo(frame);
        cv::Mat mask, result_1, result_2;
        cv::inRange(frame, cv::Scalar(0, 0, 0), low_bound, mask);
        frame.copyTo(result_1, mask);
        cv::inRange(frame, high_bound, cv::Scalar(255, 255, 255), mask);
        frame.copyTo(result_2, mask);
        frame = result_1 + result_2;
        destination_seq[i] = frame;
    }
}
