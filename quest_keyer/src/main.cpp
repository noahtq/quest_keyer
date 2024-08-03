#include <iostream>
#include <opencv2/opencv.hpp>
#include "quest_keyer/quest_keyer.h"

using namespace std;

double Square(const double& x) {
    return x * x;
}

int main(int argc, char** argv) {
    // Quest::Keyer keyer;
    // keyer.image_seq.open("../media/test_media/videos/image_sequences/small_dog_001/small_dog_001_%04d.png");
    // keyer.chromaKey(cv::Scalar(133, 160, 69), 5);
    // keyer.image_seq.render("../media/test_media/videos/image_sequences/small_dog_001_rendered/small_dog_001_rendered_%04d.png");

    if (argc == 2) {
        double a = Square(stod(argv[1]));
        std::cout << "Square of " << argv[1] << " = " << a << std::endl;
    } else if (argc > 2) {
        std::cerr << "Too many arguments\n";
    } else {
        std::cout << "Quest Keyer\n";
    }

    return 0;
}
