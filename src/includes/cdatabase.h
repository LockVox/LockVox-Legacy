#ifndef CDATABASE_H
#define CDATABASE_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "mysql.h"

using namespace std;

class CDatabase
{
    const char* MY_HOSTNAME;
    const char* MY_DATABASE;
    const char* MY_USERNAME;
    const char* MY_PASSWORD;
    const char* MY_SOCKET;
    enum {
        MY_PORT_NO = 3306,
        MY_OPT     = 0
    };
    MYSQL     *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

public:
    CDatabase();           // Constructor
    bool execMain();  // Main Process
    string getHash(string id);

};

#endif // CDATABASE_H
