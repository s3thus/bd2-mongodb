/* 
 * File:   config.cpp
 * Author: i
 * 
 * Created on January 23, 2013, 11:59 PM
 */

#include "config.h"

config::config(void)
        : host("localhost")
        , port((int)27017)
        , db("test.projekt")
{
    parseURL();
}

config::config(string h, int p)
        : host(h)
        , port(p)
        , db("test.projekt")
{
    parseURL();
}

void config::setHost(string h)
{
    host=h;
    parseURL();
}

void config::setPort(int p)
{
    port=p;
    parseURL();
}

void config::setDB(string db)
{
    db=db;
}

config::~config(void)
{
}

void config::parseURL(void)
{
    url=host;
    url.append(":");
    url.append(itoa(port));
}

string config::getDB(void)
{
    return db;
}

string config::getURL(void)
{
    return url;
}

string config::itoa(int n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}