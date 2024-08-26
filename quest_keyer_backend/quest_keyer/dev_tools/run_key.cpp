//
// Created by Noah Turnquist on 8/26/24.
//

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "../include/quest_seq_lib.h"
#include "../keyer/quest_keyer.h"

using namespace std;
using namespace Quest;

int main(int argc, char** argv) {
    if (argc != 6) {
        cerr << "Not enough arguments specified.\n";
        cerr << "Exectuable should take arguments in form: $<exe file> <file path to be keyed> <key value 1> <key value 2> <key value 3>\n";
        return 1;
    } else {
        ImageSeq image_seq;
        SeqErrorCodes code = image_seq.open(argv[1]);
        if (code != SeqErrorCodes::Success) {
            cerr << "Couldn't find file at given path\n";
            return 1;
        }
        cv::Scalar* key_vals;
        int* threshold;
        try {
            key_vals = new cv::Scalar(stod(argv[2]), stod(argv[3]), stod(argv[4]));
        } catch (...) {
            cerr << "Something went wrong converting key values to an OpenCV scalar\n";
            return 2;
        }
        try {
            threshold = new int(stoi(argv[5]));
        } catch (...) {
            cerr << "Something went wrong converting threshold to int";
            return 2;
        }
        ChromaKey(image_seq, image_seq, *key_vals, *threshold);
        delete key_vals;
        delete threshold;

        return 0;
    }
}