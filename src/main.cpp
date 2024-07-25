#include <iostream>
#include <opencv2/opencv.hpp>

#include "quist_keyer/facilities.h"

using namespace std;

int main()
{
    try {
        QuistKeyer::ImageSeq test_video;
        QuistKeyer::StatusCode code;
        code = test_video.open("../media/examples/videos/image_sequences/small_dog_001/small_dog_001_%04d.png");
        cout << (int) code << std::endl;

        code = test_video.write("../media/examples/videos/image_sequences/small_dog_001_keyed/small_dog_001_keyed_%04d.png");
        cout << (int) code << std::endl;

        // for (int i = 0; i < 186; i++) {
        //     cv::Mat img = test_video.get_frame(i);
        //     imshow("Display window", img);
        //     int k = cv::waitKey(0);
        // }
    } catch(QuistKeyer::SeqException& e) {
        cerr << "Image Sequence Exception: " << e.what() << std::endl;
    } catch(...) {
        cerr << "An unknown exception occured.\n";
    }

    return 0;
}
