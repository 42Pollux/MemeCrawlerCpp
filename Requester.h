//
// Created by pollux on 31.10.17.
//

#ifndef MEMECRAWLER_REQUESTER_H
#define MEMECRAWLER_REQUESTER_H

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <string>


class Requester {
public:
    Requester();
    int getFileSize(std::string filePath);
    std::string getData(std::string url);
    std::string getFile(std::string ulr);
    size_t FileCallback(FILE *f, char* ptr, size_t size, size_t nmemb);

    virtual ~Requester();
};


#endif //MEMECRAWLER_REQUESTER_H
