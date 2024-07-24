#include <iostream>
#include <opencv2/opencv.hpp>

#include "quist_keyer/facilities.h"

int main()
{
    QuistKeyer::ImageSeq test_video;
    QuistKeyer::StatusCode code = test_video.open("/Users/noahturnquist/Documents/Coding/Projects/2024_projects/quist_keyer/quist_keyer/media/examples/videos/image_sequences/small_dog_001/small_dog_001_%04d.png");
    std::cout << (int) code << std::endl;

    for (int i = 0; i < 186; i++) {
        cv::Mat img = test_video.get_frame(i);
        imshow("Display window", img);
        int k = cv::waitKey(0);
    }

    return 0;
}
