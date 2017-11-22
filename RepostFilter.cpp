//
// Created by pollux on 01.11.17.
//
// TODO schnellere schleifen, immer ein x später durchsuchen, mehr optimieren :: geht evtl garnicht wie ich das denke

#include <cstring>
#include <zconf.h>
#include "RepostFilter.h"

using namespace std;

list<string> active_filter;

RepostFilter::RepostFilter() {
    this->active_filter = {};
}
RepostFilter::RepostFilter(const list<string> &restart) : restart(restart) {
    this->active_filter = restart;
}

std::list<std::string> RepostFilter::filterData(std::list<std::string> data) {
    list<string> working_data = data;
    uint a = 0;
    uint b = 0;
    cout<<"active_filter size init: "<<this->active_filter.size()<<endl;
    cout<<"working_data size init: "<<working_data.size()<<endl;
    //for(string x : working_data){
    //    cout<<"tmp: "<<x<<endl;
    //}
    //for(string x : this->active_filter){
    //    cout<<"tmp: "<<x<<endl;
    //}
    //usleep(1000000*20);
    for(string x : data){
        bool duplicate = false;
        for(string y : this->active_filter){
            if(strcmp(x.c_str(), y.c_str())==0){
                working_data.remove(x);
                //cout<<"found duplicate: "<<x<<", "<<y<<", ("<<a<<") ("<<b<<") "<<this->active_filter.size()<<endl;
                duplicate = true;
            }
            b++;
        }
        if(!duplicate) this->active_filter.push_back(x);
        a++;
    }
    //cout<<"active_filter size bef: "<<this->active_filter.size()<<endl;
    if(this->active_filter.size()==0){
        for(string x : working_data){
            this->active_filter.push_back(x);
        }
    }
    cout<<"active_filter size: "<<this->active_filter.size()<<endl;
    if(this->active_filter.size()>100){                                                   // delete the oldest entries of active_filter
        for(int i = 0; i < this->active_filter.size()-25; i++){                           // so that 50 entries remain
            this->active_filter.pop_front();
        }
    }

    return working_data;
}



