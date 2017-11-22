//
// Created by pollux on 02.11.17.
//

#ifndef MEMECRAWLER_IMAGEHASHER_H
#define MEMECRAWLER_IMAGEHASHER_H

#include <string>
#include <vector>
#include <list>

class ImageHasher {
public:
    ImageHasher();
    std::list<double> getColorMomentHash(std::string path);
    float compareColorMomentHashes(double hash1[42], double hash2[42]);
private:
    std::string filePath;
};


#endif //MEMECRAWLER_IMAGEHASHER_H
