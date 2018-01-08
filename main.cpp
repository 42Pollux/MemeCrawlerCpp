#include <iostream>
#include <stdio.h>
#include <cstring>
#include <zconf.h>
#include <sstream>
#include <pthread.h>

#include "Requester.h"
#include "JSONFilter.h"
#include "RepostFilter.h"
#include "ImageHasher.h"
#include "DatabaseHandler.h"

#include "mysql_connection.h"
#include "mysql_driver.h"
#include "mysql_error.h"

#include <opencv2/core.hpp>
#include <opencv2/img_hash.hpp>

using namespace std;

static bool STOPFLAG = false;

void* ConsoleInput(void *threadid){
    while(true){
        string command;
        cin>>command;
        if(strcmp(command.c_str(), "stop")==0){
            STOPFLAG = true;
            pthread_exit(NULL);
        }
    }
}

int main(int argc, char* argv[]) {

    string dbUsername = "root";
    string dbPassword = "root";
    string localPath = argv[0];
    bool DEBUG = false;
    bool RESTART = false;
    bool THUMBNAIL = false;
    uint ticks = 600;

    for(int i=1; i<argc; i++){
        if(strcmp(argv[i], "-u")==0){
            // mysql username
            dbUsername = argv[i+1];
        }
        else if(strcmp(argv[i], "-p")==0){
            // mysql password
            dbPassword = argv[i+1];
        }
        else if(strcmp(argv[i], "--restart")==0){
            // [OPTIONAL]
            // used for restarts, ignores the initial request data
            RESTART = true;
            cout<<"[ARG]: Restarting crawler"<<endl;
        }
        else if(strcmp(argv[i], "--tick")==0){
            // [OPTIONAL]
            // time between the requests
            ticks = stoi(argv[i+1]);
            cout<<"[ARG]: Tickrate set to "<<ticks<<" seconds"<<endl;
        }
        else if(strcmp(argv[i], "--thumbnail")==0){
            // [OPTIONAL]
            // activates thumbnail saving
            THUMBNAIL = true;
            cout<<"[ARG]: Thumbnails enabled"<<endl;
        }
        else if(strcmp(argv[i], "--debug")==0){
            // [OPTIONAL]
            // used for debugging
            DEBUG = true;
            cout<<"[ARG]: Debug mode"<<endl;
        }
        else if(strcmp(argv[i], "--local")==0){
            // [OPTIONAL]
            // sets the local path
            localPath = argv[i+1];
            cout<<"[ARG]: Local path set to "<<localPath<<endl;
        }
        else if(strcmp(argv[i], "--help")==0){
            // displays all available starting options
            cout<<"Usage: MemeCrawler [options...]"<<endl;
            cout<<"   -u USER       Username used for mysql login"<<endl;
            cout<<"   -p PASSWD     Password used for mysql login"<<endl;
            cout<<"   --restart     Ignores the initial request data"<<endl;
            cout<<"   --tick SEC    Sets time between requests (refreshrate)"<<endl;
            cout<<"   --thumbnail   Creates a 300x300 thumbnail of each image"<<endl;
            cout<<"   --local PATH  Saves img/thumb to specified path"<<endl;
        }
    }

    if(argc==1){
        cout<<"MemeCrawler: try 'MemeCrawler --help' for more information"<<endl;
    }

    pthread_t consoleInputThread;
    if(pthread_create(&consoleInputThread, NULL, ConsoleInput, (void*)1)){
        cout<<"[ERR]: unable to create new thread (1)\n       Exit via CTRL+C"<<endl;
    }

    //Requester* req = new Requester();
    Requester req;
    JSONFilter jsonFilter;
    RepostFilter* repostFilter[5];
    ImageHasher hasher;
    DatabaseHandler* dbHandler = new DatabaseHandler(dbUsername, dbPassword);
    string subreddits[] = {"memes", "dankmemes", "dankchristianmemes", "meirl", "BikiniBottomTwitter"};
    //string subreddits[] = {"memes"};

    for(int i=0; i<5; i++){
        repostFilter[i] = new RepostFilter();
    }

    while(true){
        int filterIndex = 0;
        for(string s : subreddits){
            DuoList allHashes = dbHandler->getHashList();
            int addedCounter = 0;
            string response = req.getData("https://www.reddit.com/r/" + s + "/.json");
            list<string> responseList = jsonFilter.filterData(0, response);
            if(RESTART){
                repostFilter[filterIndex] = new RepostFilter(responseList);
            }
            //cout<<"filter index: "<<filterIndex<<", address: "<<&repostFilter[filterIndex]<<endl;
            responseList = repostFilter[filterIndex]->filterData(responseList);

            // Process each image link
            for (string x : responseList) {
                bool FOUND_SIMILAR = false;
                float comparison = 0.0f;
                int imageID = 0;
                char longpost = 'N';

                // Download image and get colorMomentHash
                string dlImagePath = req.getFile(x, localPath);
                if(strcmp(dlImagePath.c_str(), "")==0){
                    cout<<"\033[33mSkipped "<<x<<" (download)\033[0m"<<endl;
                    continue;
                }

                // Try to get the ColorMomentHash of the current image
                vector<double> hashes = vector<double>();
                try {
                    hashes = hasher.getColorMomentHash(dlImagePath);
                } catch (std::exception &hashExc){
                    cout<<"\033[33mSkipped "<<x<<" (hashing)\033[0m"<<endl;
                    continue;
                }

                // Check for longposts
                if(hasher.isLongpost(dlImagePath)){
                    longpost = 'Y';
                }

                // Compare each ColorMomentHash of our database with the new one
                double original_hash[42];
                for(int i=0; i<42; i++){
                    original_hash[i] = hashes[i];
                }
                cv::Mat hashA = cv::Mat(1, 42, CV_64F, &original_hash);

                for(int i = 0; i<allHashes.hashes.size(); i++){
                    double data[42];
                    for(int j=0; j<42; j++){
                        data[j] = allHashes.hashes[i].value[j];
                    }
                    cv::Mat hashB = cv::Mat(1, 42, CV_64F, &data);
                    float a = 100.0f;
                    a = hasher.compareColorMomentHashes(hashA, hashB);
                    if(a < 0.55f){
                        FOUND_SIMILAR = true;
                        imageID = i;
                        comparison = a;
                        break;
                    }
                }

                // Create a thumbnail and replace it with the url
                if((!FOUND_SIMILAR)&&THUMBNAIL){
                    string tmp = dlImagePath;
                    while(tmp.find("/")!=std::string::npos){
                        tmp = tmp.substr(tmp.find("/")+1);
                    }
                    tmp = localPath + "/thumb_" + tmp;
                    cv::Size size(200, 200);
                    cv::Mat dst;
                    cv::Mat src = cv::imread(dlImagePath);
                    cv::resize(src, dst, size);
                    remove(dlImagePath.c_str());
                    cv::imwrite(tmp, dst);

                } else {
                    remove(dlImagePath.c_str());
                }

                // Prepare the hash for the database and add if new
                if(!FOUND_SIMILAR){
                    string hash = "";
                    for (double h : hashes) {
                        ostringstream strs;
                        strs << h;
                        hash = hash + strs.str() + " ";
                    }
                    string tmp = dlImagePath;
                    while(tmp.find("/")!=std::string::npos){
                        tmp = tmp.substr(tmp.find("/")+1);
                    }
                    tmp = "thumb_" + tmp;
                    dbHandler->addEntry(x, tmp, longpost, hash);
                    hash = "HASH_PLACEHOLDER";
                    if(DEBUG){cout<<"[DAB]: Added: NULL, " + x + ", " + tmp + ", " + longpost + ", "<<hash<< endl;}
                    addedCounter++;
                } else {
                    if(DEBUG){cout<<"\033[33m[REP]: Repost detected! Image skipped ("<<x<<")->("<<allHashes.url[imageID]<<")("<<comparison<<")\033[0m"<<endl;}
                }



            }
            filterIndex++;
            cout << "[DAB]: Added " << addedCounter << " new entries!" << endl;
            cout << endl;
        }
        RESTART = false;
        for(int i=0; i<ticks; i++){
            usleep(1000000);
            if(STOPFLAG) break;
        }
        if(STOPFLAG) break;

    } // end of loop

    for(size_t i = 0; i<5; i++){
        delete repostFilter[i];
    }
    delete dbHandler;
    //delete &req;

    return 0;
}
