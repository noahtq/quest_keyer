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

void Quest::Despill(const ImageSeq& original_seq, ImageSeq& destination_seq, const cv::Scalar& key_value) {
    assert(key_value[0] > 0 || key_value[1] > 0 || key_value[2] > 0);
    int despill_channel = 0;
    int max_val = static_cast<int>(key_value[0]);
    for (int i = 1; i < 2; i++) {
        if (key_value[i] > max_val) {
            despill_channel = i;
            max_val = static_cast<int>(key_value[i]);
        }
    }

    const int other_channel_1 = (despill_channel - 1) % 3;
    const int other_channel_2 = (despill_channel + 1) % 3;
    for (int i = 0; i < original_seq.get_frame_count(); i++) {
        cv::MatIterator_<cv::Vec3b> orig_it, orig_end, dest_it;
        for (orig_it = original_seq.get_frame(i).begin<cv::Vec3b>(), dest_it = destination_seq.get_frame(i).begin<cv::Vec3b>(),
            orig_end = original_seq.get_frame(i).end<cv::Vec3b>(); orig_it < orig_end; ++orig_it, ++dest_it) {
            const auto average_value = static_cast<uchar>((static_cast<double>((*orig_it)[other_channel_1] + (*orig_it)[other_channel_2])) / 2);
            (*dest_it)[despill_channel] = (*orig_it)[despill_channel] > average_value ? average_value : (*orig_it)[despill_channel];
        }
    }
}

