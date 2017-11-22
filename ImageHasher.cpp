//
// Created by pollux on 02.11.17.
//

#include <list>
#include <opencv2/core.hpp>     //x
#include <opencv2/core/ocl.hpp>
#include <opencv2/highgui.hpp>  //x
#include <opencv2/img_hash.hpp> //x
#include <opencv2/imgproc.hpp>  //x
#include "ImageHasher.h"

ImageHasher::ImageHasher() {}

std::list<double> ImageHasher::getColorMomentHash(std::string path) {
    cv::Ptr<cv::img_hash::ImgHashBase> algo = cv::img_hash::ColorMomentHash::create();
    cv::Mat input = cv::imread(path);
    cv::Mat hash_input;

    algo->compute(input, hash_input);
    std::list<double> conversionArray;
    for(int i = 0; i<42; i++){
        conversionArray.push_back(reinterpret_cast<double &&>(hash_input.at<double>(i)));
    }

    return conversionArray;
}

float ImageHasher::compareColorMomentHashes(double *hash1, double *hash2) {
    cv::Ptr<cv::img_hash::ImgHashBase> algo = cv::img_hash::ColorMomentHash::create();
    cv::Mat hash_original, hash_var;
    hash_original = cv::Mat(1, 42, CV_64F, &hash1);
    hash_var = cv::Mat(1, 42, CV_64F, &hash2);

    return algo->compare(hash_original, hash_var);
}

