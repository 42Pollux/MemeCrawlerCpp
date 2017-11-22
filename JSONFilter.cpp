//
// Created by pollux on 31.10.17.
//

#include <list>
#include <cstring>
#include "JSONFilter.h"

using namespace std;

const size_t SUBREDDIT_MEMES = 0;   // /r/memes, /r/dankmemes, /r/dankchristianmemes
                                    // /r/meirl, /r/BikiniBottomTwitter all the same

JSONFilter::JSONFilter() {}

std::list<std::string> JSONFilter::filterData(size_t subredditID, std::string data) {

    //vector<std::string> links;
    list<string> links;

    switch(subredditID){
        case SUBREDDIT_MEMES : {
            size_t pos = 0;
            // case specific tokens for filtering
            string token_init = "\"children\":";
            string token_iter = "\"hide_score\": ";
            string token_data = "\"url\": \"";
            string iter_end   = "\",";

            // steps to filter out the image links
            string str_init = data.substr(data.find(token_init));           // new string that starts at token_init
            string str_iter = str_init.substr(str_init.find(token_iter));   // new string that starts at token_iter
            pos = str_iter.find(token_iter);
            while(pos!=string::npos){
                str_iter = str_iter.substr(pos);                            // jump to token_iter in str_iter
                str_iter = str_iter.substr(str_iter.find(token_data));      // jumps to token_data in str_iter
                string link = str_iter.substr(8, str_iter.find(iter_end)-8);// grabs link out of json
                pos = str_iter.find(token_iter);                            // sets pos to next position of token_iter or to npos

                string str_link_end = link.substr(link.length()-5, 5);      // these steps check for a valid image link
                if(str_link_end.find(".")!=string::npos){
                    str_link_end = str_link_end.substr(str_link_end.find("."));
                    if((strcmp(".jpg", str_link_end.c_str())==0)||(strcmp(".jpeg", str_link_end.c_str())==0)||(strcmp(".png", str_link_end.c_str())==0)) {
                        links.push_back(link);
                    }
                }
            }
            return links;
        }


    }
    return links;
}


