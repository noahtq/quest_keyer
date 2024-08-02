#include <iostream>
#include <opencv2/opencv.hpp>
#include "../include/quest_seq_lib.h"

using namespace std;

int main()
{
    Quest::ImageSeq seq;
    seq.open("../media/test_media/videos/image_sequences/small_dog_001/small_dog_001_%04d.png");
    for (cv::Mat frame : seq) {
        cv::GaussianBlur(frame, frame, cv::Size(55, 55), 0, 0, cv::BORDER_CONSTANT);
    }
    seq.render("../media/test_media/videos/image_sequences/small_dog_001_rendered/small_dog_001_rendered_%04d.png");

    return 0;
}
