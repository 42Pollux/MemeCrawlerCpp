//
// Created by pollux on 11.11.17.
//

#ifndef MEMECRAWLER_DATABASEHANDLER_H
#define MEMECRAWLER_DATABASEHANDLER_H

//#include "mysql_connection.h"
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


typedef struct DuoList DuoList;
typedef struct ExtendedList ExtendedList;

class DatabaseHandler {
public:
    DatabaseHandler(const std::string &username, const std::string &password);
    void addEntry(std::string link, char longpost, std::string hash);
    DuoList getHashList();
    virtual ~DatabaseHandler();

private:
    std::string username;
    std::string password;

    sql::Driver *driver;
    sql::Connection *con;

};

struct ExtendedList {
    double value[42];
};

struct DuoList {
    std::vector<int> id;
    std::vector<ExtendedList> hashes;
};



#endif //MEMECRAWLER_DATABASEHANDLER_H
