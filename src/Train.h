/**
    Provide by Walon Li

    File: Train.h
**/
#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <vector>
#include <QTime>
#include "basic.h"

using std::string ;
using std::vector ;
using std::pair ;

namespace ttp
{

class Train
{
public:
    explicit Train( const string & t, const string & h) : id(t), help(h) { data.clear(); }
    ~Train(){}

    typedef string arrival_station ;
    typedef QTime arrival_time ;

    // operator overload check
    bool operator !()
    {
        return (id.empty() || data.empty()) ? true : false ;
    }
    bool operator !() const
    {
        return (id.empty() || data.empty()) ? true : false ;
    }


    const string & get_id() { return id ; }
    const string & get_help() { return help ;}
    const vector< pair<arrival_station, arrival_time> > & get_data() { return data ;}

    STATE add_data_into_train(const pair<arrival_station, arrival_time> d)
    {
        if (d.first.empty())
            return STATE_DATA_ERROR ;
        else
        {
            data.push_back(d);
            return STATE_SUCCESS ;
        }
    }

private:
    string id ;
    string help ;
    vector< pair<arrival_station, arrival_time> > data;
};

}
#endif // ITRAIN_H
