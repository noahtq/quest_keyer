//
// Created by Noah Turnquist on 7/22/24.
//

#ifndef FACILITIES_H
#define FACILITIES_H
#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <utility>

namespace QuistKeyer {
    enum class StatusCode {success = 200, invalid = 400, forbidden = 403, path_not_found = 404};
    const std::vector<std::string> output_image_extensions = {".png"};

    class Status {
        StatusCode code;
        std::string message;
    public:
        explicit Status(const StatusCode new_code, std::string new_message = "")
            : code(new_code), message(std::move(new_message)) {}
    };

    class SeqException: public std::exception {
        std::string message;
    public:
        explicit SeqException(const char* msg)
            : message(msg) {}

        [[nodiscard]] const char* what() const throw() {
            return message.c_str();
        }
    };

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
        Status open(const std::filesystem::path& new_input_path);
        Status write(const std::filesystem::path& new_output_path);
    };

    std::filesystem::path ConvertPathToFramePath(const std::filesystem::path& input_path, const int& frame_num);
}

#endif //FACILITIES_H
