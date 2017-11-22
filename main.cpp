#include <iostream>
#include <cstring>
#include <zconf.h>
#include <sstream>

#include "Requester.h"
#include "JSONFilter.h"
#include "RepostFilter.h"
#include "ImageHasher.h"
#include "DatabaseHandler.h"

#include "mysql_connection.h"
#include "mysql_driver.h"
#include "mysql_error.h"

using namespace std;

int main(int argc, char* argv[]) {

    string dbUsername = "root";
    string dbPassword = "root";
    string localPath;
    bool restart = false;
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
        else if(strcmp(argv[i], "--local")==0){
            // [OPTIONAL]
            // save memes locally at '--local PATH'
            // no thumbnails, no mysql entries
            localPath = argv[i+1];
        }
        else if(strcmp(argv[i], "--restart")==0){
            // [OPTIONAL]
            // used for restarts, ignores the initial request data
            restart = true;
        }
        else if(strcmp(argv[i], "--tick")==0){
            // [OPTIONAL]
            // time between the requests
            ticks = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "--debug")==0){
            // [OPTIONAL]
            // used for debugging
        }
        else if(strcmp(argv[i], "--help")==0){
            // displays all available starting options
            cout<<"Usage: MemeCrawler [options...]"<<endl;
            cout<<"   -u USER       Username used for mysql login"<<endl;
            cout<<"   -p PASSWD     Password used for mysql login"<<endl;
            cout<<"   --restart     Ignores the initial request data"<<endl;
            cout<<"   --tick SEC    sets time between requests (refreshrate)"<<endl;
            cout<<"   --local PATH  Saves memes only locally at the specified path"<<endl;
        }
    }

    if(argc==1){
        cout<<"MemeCrawler: try 'MemeCrawler --help' for more information"<<endl;
    }

    //Requester* req = new Requester();
    Requester req;
    JSONFilter jsonFilter;
    RepostFilter* repostFilter[5];
    ImageHasher hasher;
    DatabaseHandler* dbHandler = new DatabaseHandler(dbUsername, dbPassword);
    //string subreddits[] = {"memes", "dankmemes", "dankchristianmemes", "meirl", "BikiniBottomTwitter"};
    string subreddits[] = {"memes"};

    for(int i=0; i<5; i++){
        repostFilter[i] = new RepostFilter();
    }

    while(true){
        int filterIndex = 0;
        for(string s : subreddits){
            string response = req.getData("https://www.reddit.com/r/" + s + "/.json");
            list<string> responseList = jsonFilter.filterData(0, response);
            if(restart){
                repostFilter[filterIndex] = new RepostFilter(responseList);
            }
            cout<<"filter index: "<<filterIndex<<", address: "<<&repostFilter[filterIndex]<<endl;
            responseList = repostFilter[filterIndex]->filterData(responseList);

            // process each image link
            for (string x : responseList) {
                //download image and get colorMomentHash
                string dlImagePath = req.getFile(x);
                if(strcmp(dlImagePath.c_str(), "")==0){
                    cout<<"Skipped "<<x<<" (download)"<<endl;
                    continue;
                }
                //cout << x << endl;
                //cout<<dlImagePath<<endl;
                list<double> hashes;
                try {
                    hashes = hasher.getColorMomentHash(dlImagePath);
                } catch (std::exception &hashExc){
                    cout<<"Skipped "<<x<<" (hashing)"<<endl;
                    continue;
                }
                string hash = "";
                for (double h : hashes) {
                    ostringstream strs;
                    strs << h;
                    hash = hash + strs.str() + " ";
                }
                // TODO check db for smiliar hash, add to db
                dbHandler->addEntry(x, 'Y', hash);
                cout << "[DB] Add: NULL, " + x + ", Y, "<<hash<< endl;

            }
            DuoList dbResponse = dbHandler->getHashList();
            cout<<dbResponse.hashes.size()<<endl;
            cout<<"HASH 21: ";
            for(double a : dbResponse.hashes.at(20).value){
                cout<<a<<", ";
            }
            cout<<endl;
            filterIndex++;
            cout << "added " << responseList.size() << " new entries" << endl;
            cout << endl;
        }
        cout<<"temp stop: ";
        std::string whatever;
        cin>>whatever;
        if(strcmp(whatever.c_str(), "stop")==0) break;
        for(int i=0; i<ticks; i++){
            usleep(1000000);
        }

    }
    for(size_t i = 0; i<5; i++){
        delete repostFilter[i];
    }
    delete &dbHandler;
    delete &req;

    return 0;
}