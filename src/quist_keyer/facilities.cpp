//
// Created by Noah Turnquist on 7/22/24.
//

#include "facilities.h"

QuistKeyer::Status QuistKeyer::ImageSeq::open(const std::filesystem::path& new_input_path) {
    input_video.open(new_input_path, cv::CAP_IMAGES);
    if (!input_video.isOpened()) {
        return Status(StatusCode::path_not_found, "Could not find path");
    }
    input_path = new_input_path;
    output_frames.resize(static_cast<int>(input_video.get(cv::CAP_PROP_FRAME_COUNT)));
    // Copy all of the frames in the video writer to a vector of matrices.
    // All image manipulation will be done on those.
    for(int i = 0; i < input_video.get(cv::CAP_PROP_FRAME_COUNT); i++) {
        cv::Mat video_frame;
        input_video >> video_frame;
        video_frame.copyTo(output_frames[i]);
    }

    return Status(StatusCode::success, "Successfully opened image");
}

QuistKeyer::Status QuistKeyer::ImageSeq::write(const std::filesystem::path& new_output_path) {
    if (output_frames.empty()) {
        throw SeqException("Attempting to write out an image sequence before it has been opened");
    }

    if (!is_directory(new_output_path.parent_path())) {
        return Status(StatusCode::path_not_found, "No directory exists");
    }

    const std::string extension = new_output_path.extension();
    for (const std::string& valid : output_image_extensions) {
        if (extension == valid) {
            for (int i = 0; i < output_frames.size(); i++) {
                std::filesystem::path frame_path = ConvertPathToFramePath(new_output_path.parent_path(), i + 1);
                cv::imwrite(frame_path, output_frames[i]);
            }
            return Status(StatusCode::success, "Image rendered successfully");
        }
    }

    return Status(StatusCode::invalid, "Image extension is not supported");
}

std::filesystem::path QuistKeyer::ConvertPathToFramePath(const std::filesystem::path& input_path, const int& frame_num) {
    std::stringstream ss;
    ss << "small_dog_001_keyed_" << std::setfill('0') << std::setw(4) << frame_num << ".png";
    return input_path / ss.str(); // TODO: finish this function
}
