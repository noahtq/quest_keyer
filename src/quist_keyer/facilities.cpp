//
// Created by Noah Turnquist on 7/22/24.
//

#include "facilities.h"

QuistKeyer::ImageSeq::ImageSeq(const std::filesystem::path& new_input_path) {
    input_image = cv::imread(new_input_path);
    input_path = new_input_path;
}
