//
// Created by pollux on 01.11.17.
//

#ifndef MEMECRAWLER_REPOSTFILTER_H
#define MEMECRAWLER_REPOSTFILTER_H

#include <iostream>
#include <list>

class RepostFilter {
    std::list<std::string> restart;
    std::list<std::string> active_filter;
public:


    RepostFilter(const std::list<std::string> &restart);

    RepostFilter();

    std::list<std::string> filterData(std::list<std::string> data);
};


#endif //MEMECRAWLER_REPOSTFILTER_H
