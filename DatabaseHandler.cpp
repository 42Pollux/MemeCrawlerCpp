//
// Created by pollux on 11.11.17.
//

#include "DatabaseHandler.h"

//#include "mysql_connection.h"
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

DatabaseHandler::DatabaseHandler(const std::string &username, const std::string &password) : username(username),
                                                                                             password(password) {
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", username, password);
}

DuoList DatabaseHandler::getHashList() {
    sql::Statement *stmt;
    sql::ResultSet *res;
    DuoList hashList;

    stmt = con->createStatement();
    stmt->execute("USE memes");
    res = stmt->executeQuery("SELECT id, hash FROM memerepos");
    while(res->next()){
        hashList.id.push_back(res->getInt("id"));
        std::string hashString = res->getString("hash");
        //std::cout<<"hashString: "<<hashString<<std::endl;
        size_t pos = 0, j = 0;
        ExtendedList extList;
        while(j<42){ //pos!=std::string::npos
            std::string tmp = hashString.substr(0, hashString.find(" "));
            extList.value[j] = atof(tmp.c_str());
            //std::cout<<"("<<j<<") "<<extList.value[j]<<" ("<<tmp.c_str()<<")"<<std::endl;
            pos = hashString.find(" ")+1;
            hashString = hashString.substr(pos);
            j++;
        }

        //std::cout<<"hererer"<<std::endl;
        hashList.hashes.push_back(extList);
    }
    delete stmt;
    delete res;
    return hashList;
}

DatabaseHandler::~DatabaseHandler() {
    //delete driver;
    //delete con;

}

void DatabaseHandler::addEntry(std::string link, char longpost, std::string hash) {
    sql::Statement *stmt;
    //sql::ResultSet *res;

    stmt = con->createStatement();
    //stmt->execute("USE memes");
    con->setSchema("memes");
    //std::cout<<"INSERT INTO memerepos(id, url, longpost, hash) VALUES (NULL, '" + link + "', '" + longpost + "', '" + hash + "')"<<std::endl;;

    try {
        stmt->execute("INSERT INTO memerepos(id, url, longpost, hash) VALUES(NULL, '" + link + "', '" + longpost + "', '" + hash + "')");
    } catch (sql::SQLException &e) {
        using namespace std;
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    } catch (std::exception &stde){
        std::cout<<stde.what()<<std::endl;
    }
    delete stmt;
    //delete res;
}
