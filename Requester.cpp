//
// Created by pollux on 31.10.17.
//

#define SKIP_PEER_VERIFICATION = true
#define SKIP_HOSTNAME_VERIFICATION = false

#include "Requester.h"
#include <curl/curl.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>


Requester::Requester() {
    std::cout<<"Initializing 'Requester' object and curlpp"<<std::endl;
    curlpp::initialize(CURL_GLOBAL_ALL);
    std::cout<<"Requester/curl initialized"<<std::endl;
}

Requester::~Requester() {
    curlpp::terminate();
}

std::string Requester::getData(std::string url) {
    //curlpp::Cleanup myCleanup;
    curlpp::Easy conn;

    //std::cout<<"[REQ]: Requesting json from "<<url<<std::endl;

    std::stringstream ss;
    curlpp::Options::WriteStream ws(&ss);
    conn.setOpt<curlpp::Options::Url>(url);
    conn.setOpt(ws);
    try {
        conn.perform();
    } catch (curlpp::RuntimeError & e){
        std::cout<<e.what()<<std::endl;
    } catch (curlpp::LogicError & e){
        std::cout<<e.what()<<std::endl;
    }

    std::cout<<"\033[32m[REQ]: Request successful ("<<url<<")\033[0m"<<std::endl;
    return ss.str();
}
size_t Requester::FileCallback(FILE *f, char* ptr, size_t size, size_t nmemb)
{
    return fwrite(ptr, size, nmemb, f);
}




std::string Requester::getFile(std::string url, std::string localPath) {
    curlpp::Cleanup myCleanup;
    curlpp::Easy conn;

    std::string ending = url.substr(url.length()-5, 5);
    ending = ending.substr(ending.find("."));
    ending = ending.substr(1, ending.length()-1);

    std::string newname = url;
    while(newname.find("/")!=std::string::npos){
        newname = newname.substr(newname.find("/")+1);
    }
    newname = newname.substr(0, newname.find("."));

    std::string outputpath = localPath + "/" + newname + "." + ending;
    //std::cout<<outputpath<<std::endl;

    FILE* file = fopen(outputpath.c_str(), "w");
    if(!file){
        std::cout<<"Couldn't create/access file"<<std::endl;
        return "";
    }

    using namespace std::placeholders;
    curlpp::options::WriteFunction *test = new curlpp::options::WriteFunction(std::bind(&Requester::FileCallback, this, file, _1, _2, _3));
    //auto abc = std::bind(&Requester::FileCallback, this, file, _1, _2, _3);
    conn.setOpt(test);
    conn.setOpt(new curlpp::options::Url(url));
    conn.setOpt(new curlpp::options::Verbose(false));
    try{
        conn.perform();
    } catch ( curlpp::LogicError & e )
    {
        std::cout << e.what() << std::endl;
        return "";
    } catch ( curlpp::RuntimeError & e )
    {
        std::cout << e.what() << std::endl;
        return "";
    }

    fclose(file);
    int size = getFileSize(outputpath);
    if(size==0){
        //std::cout<<"Error receiving file ("<<size<<")"<<std::endl;
        if(url.find("i.imgur")==std::string::npos){                                 // imgur patch (temporary)
            if(url.find("imgur")!=std::string::npos){                               // //imgur.com = bad
                std::string tmp = url.substr(0, url.find("//"));                    // //i.imgur = good
                url = tmp + "//i." + url.substr(url.find("imgur"));
                //std::cout<<"NEW URL: "<<url<<std::endl;
                return getFile(url, localPath);
            }
        }
        return "";
    }

    return outputpath;
}

int Requester::getFileSize(std::string filePath) {
    using namespace std;
    streampos begin, end;
    ifstream file (filePath, ios::binary);
    begin = file.tellg();
    file.seekg(0, ios::end);
    end = file.tellg();
    file.close();
    return (end-begin);
}


