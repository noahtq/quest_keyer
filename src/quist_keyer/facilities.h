//
// Created by Noah Turnquist on 7/22/24.
//

#ifndef FACILITIES_H
#define FACILITIES_H
#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace QuistKeyer {
    class ImageSeq {
        std::filesystem::path input_path = "";
        std::filesystem::path output_path = "";
        cv::Mat input_image;
        cv::Mat output_image;
    public:
        ImageSeq(const std::filesystem::path& new_input_path);
    };

    class KeyContainer {

    };
}

#endif //FACILITIES_H
