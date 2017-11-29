//
// Created by pollux on 02.11.17.
//

#include <list>
#include <opencv2/core.hpp>     //x
#include <opencv2/core/ocl.hpp>
#include <opencv2/highgui.hpp>  //x
#include <opencv2/img_hash.hpp> //x
#include <opencv2/imgproc.hpp>  //x
#include <iostream>
#include "ImageHasher.h"

ImageHasher::ImageHasher() {}

std::vector<double> ImageHasher::getColorMomentHash(std::string path) {
    cv::Ptr<cv::img_hash::ImgHashBase> algo = cv::img_hash::ColorMomentHash::create();
    cv::Mat input = cv::imread(path);
    cv::Mat hash_input;

    algo->compute(input, hash_input);
    std::vector<double> conversionArray;
    for(int i = 0; i<42; i++){
        conversionArray.push_back(reinterpret_cast<double &&>(hash_input.at<double>(i)));
    }

    return conversionArray;
}

float ImageHasher::compareColorMomentHashes(cv::Mat hash1, cv::Mat hash2) {
    cv::Ptr<cv::img_hash::ImgHashBase> algo = cv::img_hash::ColorMomentHash::create();
    float ret = 100.0f;
    try {
        ret = algo->compare(hash1, hash2);
    } catch (std::exception &e){
        std::cout<<e.what()<<std::endl;
    }

    return algo->compare(hash1, hash2);
}

