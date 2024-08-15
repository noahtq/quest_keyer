//
// Created by Noah Turnquist on 8/2/24.
//

#include "quest_keyer.h"

void Quest::ChromaKey(const ImageSeq& original_seq, ImageSeq& destination_seq, const cv::Scalar& key_value, const double& threshold) {
    // TODO: Make this color agnostic

    const int key_channel = 1; // Green
    const int other_channel = 0; // Blue

    cv::Size frame_size = cv::Size(original_seq.get_frame(0).cols, original_seq.get_frame(0).rows);

    for (int i = 0; i < original_seq.get_frame_count(); i++) {
        // Define original and destination frames and convert to 32 bit float matrices
        // with color values between 0 and 1
        cv::Mat orig_frame = original_seq.get_frame(i);
        cv::Mat& dest_frame = destination_seq[i];
        orig_frame.convertTo(orig_frame, CV_32FC3, 1.0/255.0);
        dest_frame.convertTo(dest_frame, CV_32FC3, 1.0/255.0);

        // A color plane that the foreground colors lie on
        const cv::Scalar background_color(key_value[0] / 255.0, key_value[1] / 255.0, key_value[2] / 255.0);
        const cv::Scalar A(0, 1, -threshold, 0);
        const double k1 = 1 / background_color.dot(A);
        cv::MatIterator_<cv::Vec3f> orig_it, orig_end;
        cv::MatIterator_<cv::Vec4f> dest_it;
        for (orig_it = orig_frame.begin<cv::Vec3f>(), dest_it = dest_frame.begin<cv::Vec4f>(),
            orig_end = orig_frame.end<cv::Vec3f>(); orig_it < orig_end; ++orig_it, ++dest_it) {
            const float alpha_val = std::clamp(1 - k1 * ((*orig_it)[key_channel] - threshold * (*orig_it)[other_channel]), 0.0, 1.0);
            for (int c = 0; c < 3; c++) {
                (*dest_it)[c] = (*orig_it)[c] * alpha_val;
            }
            }
        orig_frame.convertTo(orig_frame, CV_8U, 255);
        dest_frame.convertTo(dest_frame, CV_8U, 255);
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

