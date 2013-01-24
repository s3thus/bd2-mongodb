/* 
 * File:   config.h
 * Author: i
 *
 * Created on January 23, 2013, 11:59 PM
 */
#pragma once
#ifndef CONFIG_H
#define	CONFIG_H
#include <string>
#include <sstream>
using namespace std;

class config
{
public:
    config(void);
    config(string h, int p);
    void setHost(string h);
    void setPort(int p);
    void setDB(string db);
    string getDB(void);
    string getURL(void);
    ~config();
private:
    string url;
    string host;
    int port;
    string db;
    void parseURL(void);
    string itoa(int n);
};

#endif	/* CONFIG_H */

