//
// Created by pollux on 31.10.17.
//

#ifndef MEMECRAWLER_JSONFILTER_H
#define MEMECRAWLER_JSONFILTER_H

#include <iostream>
#include <vector>

class JSONFilter {
public:
    JSONFilter();
    std::list<std::string> filterData(size_t subredditID, std::string data);
};


#endif //MEMECRAWLER_JSONFILTER_H
