//
// Created by pollux on 02.11.17.
//

#ifndef MEMECRAWLER_IMAGEHASHER_H
#define MEMECRAWLER_IMAGEHASHER_H

#include <string>
#include <vector>
#include <list>

#include <opencv2/core.hpp>     //x
#include <opencv2/core/ocl.hpp>
#include <opencv2/highgui.hpp>  //x
#include <opencv2/img_hash.hpp> //x
#include <opencv2/imgproc.hpp>  //x

class ImageHasher {
public:
    ImageHasher();
    std::vector<double> getColorMomentHash(std::string path);
    float compareColorMomentHashes(cv::Mat hash1, cv::Mat hash2);
    bool isLongpost(std::string path);
private:
    std::string filePath;
};


#endif //MEMECRAWLER_IMAGEHASHER_H
