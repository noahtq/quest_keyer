#include <iostream>
#include <opencv2/opencv.hpp>
#include "quest_keyer/quest_keyer.h"

using namespace std;

int main()
{
    Quest::Keyer keyer;
    keyer.image_seq.open("../media/test_media/videos/image_sequences/small_dog_001/small_dog_001_%04d.png");
    keyer.chromaKey(cv::Scalar(133, 160, 69));
    keyer.image_seq.render("../media/test_media/videos/image_sequences/small_dog_001_rendered/small_dog_001_rendered_%04d.png");

    return 0;
}
