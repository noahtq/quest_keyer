//
// Created by Noah Turnquist on 7/22/24.
//

#ifndef FACILITIES_H
#define FACILITIES_H
#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace QuistKeyer {
    enum class StatusCode {success = 200, path_not_found = 404};

    class ImageSeq {
        std::filesystem::path input_path = "";
        std::filesystem::path output_path = "";
        cv::VideoCapture input_video;
        std::vector<cv::Mat> output_frames;
    public:
        // Constructors
        ImageSeq() = default;

        // Getters and setters
        [[nodiscard]] cv::Mat get_frame(const int& frame_num) const { return output_frames[frame_num]; }

        // Methods
        StatusCode open(const std::filesystem::path& new_input_path);
    };

    class KeyContainer {

    };
}

#endif //FACILITIES_H
