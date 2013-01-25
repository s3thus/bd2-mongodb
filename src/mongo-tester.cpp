#include <stdio.h>
#include <stdlib.h>
#include <mongo/client/dbclient.h>
#include <fstream>
#include <iostream>
#include "config.h"
#include "stopWatch.h"

using namespace std;

config cfg("s3thus.dyndns.org", 27017);

void feedMongo (mongo::DBClientConnection&, mongo::BSONObjBuilder*, int);
mongo::BSONObjBuilder* parseData(string, int&);
mongo::BSONObjBuilder* allocBSON(int);
mongo::BSONObjBuilder* deallocBSON(mongo::BSONObjBuilder*);

int main(int argc, char** argv)
{
    string filename="1k";
    stopWatch timer;
    mongo::BSONObjBuilder* bson=NULL;
    int count=0;

    // Parse command line options
    argv++; argc--;
    while (argc)
    {
        if (argc >= 2 && !strcmp(argv[0],"-h"))
        {
            argv++; argc--;
            cfg.setHost(argv[0]);
        }
        else if (argc >= 2 && !strcmp(argv[0],"-p"))
        {
            argv++; argc--;
            cfg.setPort(atoi(argv[0]));
        }
        else if (argc >= 2 && !strcmp(argv[0],"-f"))
        {
            argv++; argc--;
            filename = argv[0];
        }
        else if (argc >= 2 && !strcmp(argv[0],"-db"))
        {
            argv++; argc--;
            cfg.setDB(argv[0]);
        }
        else
        {
            fprintf(stderr, "Invalid argument: %s\n", argv[0]);
            return -1;
        }
        argv++; argc--;
    }

    bson=parseData(filename, count);
    if(bson!=NULL)
    {
        // setting up connection to db
        mongo::DBClientConnection c;
        string errmsg;
        if(!c.connect(cfg.getURL(), errmsg))
        {
            cerr << "ERROR WITH CONNECTION: " << errmsg << endl;
        }
        else
        {
            // connection set, clearing db
            mongo::BSONObjBuilder query;
            c.remove(cfg.getDB(), query.obj());
            // feeding time
            timer.startTimer();
            feedMongo(c, bson, count);
            timer.stopTimer();
            cerr << "\t" << count << "\t" << timer.getDuration() << " s\n";
        }

        bson=deallocBSON(bson);
        return 0;
    }
    else
    {
        cerr << "Something went wrong with BSON parse!\n";
        return -2;
    }
}

// Feeding parsed data into database
void feedMongo(mongo::DBClientConnection &c, mongo::BSONObjBuilder* o, int num)
{
    //cerr << "CONNECTED AND CLEARED DB\n";
    for(int i=0; i<num; i++)
    {
        c.insert(cfg.getDB(), o[i].obj());
    }
}

// Function reading file and parsing data into BSON;
// file is <key> <value> structured
mongo::BSONObjBuilder* parseData(string filename, int& n)
{
    mongo::BSONObjBuilder* b=NULL;
    unsigned i=0;
    int count;
    string tmp;
    fstream in(filename.c_str(), fstream::in);
    
    if(!in.is_open())
    {
        cerr << "ERROR WITH FILE!\n";
        return NULL;
    }
    else
    {
        in >> count;
        b=allocBSON(count);
        for(unsigned i=0; i<count; i++)
        {
            in >> tmp;
            b[i].append("key", tmp);
            in >> tmp;
            b[i].append("value", tmp);
        }
        n=count;
        return b;
    }
}

// allocating memory for table with parsed BSONs
mongo::BSONObjBuilder* allocBSON(int n)
{
    mongo::BSONObjBuilder* B=new mongo::BSONObjBuilder[n];
    return B;
}

// free memory used by table used to store BSONs
mongo::BSONObjBuilder* deallocBSON(mongo::BSONObjBuilder* B)
{
    delete[] B;
    B=NULL;
    return B;
}
