//
// Created by Noah Turnquist on 7/22/24.
//

#include "facilities.h"

QuistKeyer::StatusCode QuistKeyer::ImageSeq::open(const std::filesystem::path& new_input_path) {
    input_video.open(new_input_path, cv::CAP_IMAGES);
    if (!input_video.isOpened()) {
        return StatusCode::path_not_found;
    }
    input_path = new_input_path;

    output_frames.resize(static_cast<int>(input_video.get(cv::CAP_PROP_FRAME_COUNT)));
    for(int i = 0; i < input_video.get(cv::CAP_PROP_FRAME_COUNT); i++) {
        cv::Mat video_frame;
        input_video >> video_frame;
        video_frame.copyTo(output_frames[i]);
    }

    return StatusCode::success;
}
