//
// Created by Noah Turnquist on 8/2/24.
//

#ifndef QUEST_KEYER_H
#define QUEST_KEYER_H

#include <opencv2/opencv.hpp>
#include "../include/quest_seq_lib.h"

namespace Quest {
    class KeyerException: public std::exception {
        std::string message;
    public:
        explicit KeyerException(const char* msg)
            : message(msg) {}

        [[nodiscard]] const char* what() const throw() {
            return message.c_str();
        }
    };

    void UltimatteKeyer(const ImageSeq& original_seq, ImageSeq& destination_seq, const cv::Scalar& key_value, const double& threshold);
    void Despill(const ImageSeq& original_seq, ImageSeq& destination_seq, const cv::Scalar& key_value);
    void CompositeOverImage(const ImageSeq& original_seq, ImageSeq& destination_seq, const std::filesystem::path& bg_path);
}

#endif //QUEST_KEYER_H
