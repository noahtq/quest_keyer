#include <iostream>
#include <fstream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "../include/json.hpp"
#include "quest_keyer/quest_keyer.h"

using namespace std;
using json = nlohmann::json;

double Square(const double& x) {
    return x * x;
}

int main(int argc, char** argv) {
    // Quest::Keyer keyer;
    // keyer.image_seq.open("../media/test_media/videos/image_sequences/small_dog_001/small_dog_001_%04d.png");
    // keyer.chromaKey(cv::Scalar(133, 160, 69), 5);
    // keyer.image_seq.render("../media/test_media/videos/image_sequences/small_dog_001_rendered/small_dog_001_rendered_%04d.png");

    if (argc == 1) {
        cout << "Quest Keyer\n";
    } else if (argc == 2) {
        std::string argv_str(argv[1]);
        if (argv_str == "-json") {
            // Creating config object
            std::ifstream json_ifs("../config.json");
            if (!json_ifs) {
                std::cerr << "Couldn't open config file\n";
                return 1;
            }
            const json data = json::parse(json_ifs);
            json_ifs.close();
            const Quest::KeyerConfig config(data);

            std::ifstream request_ifs(config.temp_dir / config.frontend_file);
            if (!request_ifs) {
                std::cerr << "Couldn't open frontend data file\n";
                return 1;
            }
            json request = json::parse(request_ifs);
            request_ifs.close();

            if (request.at("command") == "init") {
                cout << request.at("payload") << std::endl;
            } else {
                cerr << argv[1] << " is not a recognized command\n";
            }
        } else {
            cerr << "Unrecognized data exchange type\n";
        }
    } else {
        cerr << "Invalid number of arguments provided. Format should be [executable] [json object]\n";
    }

    return 0;
}
