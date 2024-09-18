//
// Created by Noah Turnquist on 8/2/24.
//

#include "quest_keyer.h"

void Quest::UltimatteKeyer(const ImageSeq& original_seq, ImageSeq& destination_seq, const cv::Scalar& key_value, const double& threshold) {
    int key_channel = 0;
    int max_val = static_cast<int>(key_value[0]);
    for (int i = 1; i < 3; i++) {
        if (key_value[i] > max_val) {
            key_channel = i;
            max_val = static_cast<int>(key_value[i]);
        }
    }

    int other_channel;
    switch (key_channel) {
    case 0: // Blue
        other_channel = 1; // Green
        break;
    case 1: // Green
        other_channel = 0; // Blue
        break;
    case 2: // Red
        other_channel = 1; // Green
        break;
    default:
        throw std::out_of_range("Something went wrong. While selecting the most prominent key value");
    }

    for (int i = 0; i < original_seq.get_frame_count(); i++) {
        // Define original and destination frames and convert to 32 bit float matrices
        // with color values between 0 and 1.
        cv::Mat orig_frame = original_seq.get_frame(i);
        cv::Mat& dest_frame = destination_seq[i];
        orig_frame.convertTo(orig_frame, CV_32FC3, 1.0/255.0);
        dest_frame.convertTo(dest_frame, CV_32FC4, 1.0/255.0);

        const cv::Scalar background_color(key_value[0] / 255.0, key_value[1] / 255.0, key_value[2] / 255.0);
        const cv::Scalar A(0, 1, -threshold, 0); // A color plane that the foreground colors lie on
        const double k1 = 1 / background_color.dot(A);
        cv::MatIterator_<cv::Vec3f> orig_it, orig_end;
        cv::MatIterator_<cv::Vec4f> dest_it;
        for (orig_it = orig_frame.begin<cv::Vec3f>(), dest_it = dest_frame.begin<cv::Vec4f>(),
            orig_end = orig_frame.end<cv::Vec3f>(); orig_it < orig_end; ++orig_it, ++dest_it) {
            (*dest_it)[3] = 1.0; // Set alpha value to pure white just in case
            const float alpha_val = static_cast<float>(std::clamp(1 - k1 * ((*orig_it)[key_channel] - threshold * (*orig_it)[other_channel]), 0.0, 1.0));
            for (int c = 0; c < 4; c++) {
                (*dest_it)[c] = (*orig_it)[c] * alpha_val;
            }
            }
        dest_frame.convertTo(dest_frame, CV_8UC4, 255);
    }
}

void Quest::Despill(const ImageSeq& original_seq, ImageSeq& destination_seq, const cv::Scalar& key_value) {
    if (key_value[0] > 0 || key_value[1] > 0 || key_value[2] > 0) {
        throw KeyerException("The key value can not be pure black (0, 0, 0)");
    }
    int despill_channel = 0;
    int max_val = static_cast<int>(key_value[0]);
    for (int i = 1; i < 3; i++) {
        if (key_value[i] > max_val) {
            despill_channel = i;
            max_val = static_cast<int>(key_value[i]);
        }
    }

    const int other_channel_1 = despill_channel - 1 > 0 ? (despill_channel - 1) % 3 : 2; // Have to do this one kind of weird because of the way C++ does modulo on negative numbers
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

void Quest::CompositeOverImage(const ImageSeq& original_seq, ImageSeq& destination_seq, const std::filesystem::path& bg_path) {
    cv::Mat bg = cv::imread(bg_path);
    if (bg.empty()) {
        throw(KeyerException("Could not open background image at path"));
    }
    cv::resize(bg, bg, cv::Size(original_seq.get_width(), original_seq.get_height()), 0, 0);
    bg.convertTo(bg, CV_8UC4);
    cv::Mat bg_channels[4];
    cv::split(bg, bg_channels);
    const cv::Size frame_size = cv::Size(bg.cols, bg.rows);
    const cv::Mat pure_black(frame_size, CV_8UC1, cv::Scalar(0));
    pure_black.copyTo(bg_channels[3]);

    for (int i = 0; i < original_seq.get_frame_count(); i++) {
        cv::Mat fg_channels[4];
        cv::split(original_seq.get_frame(i), fg_channels);

        cv::Mat current_bg_channels[4];
        // Subtract the alpha value from each channel in the bg
        for (int c = 0; c < 4; c++) {
            bg_channels[c].copyTo(current_bg_channels[c]);
            current_bg_channels[c] -= fg_channels[3];
        }

        cv::Mat current_bg;
        cv::merge(current_bg_channels, 4, current_bg);
        cv::add(original_seq.get_frame(i), current_bg, destination_seq[i]);
    }
}


