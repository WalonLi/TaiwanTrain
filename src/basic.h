/**
    Provide by Walon Li

    File: state.h
**/

#ifndef STATE_H
#define STATE_H

#include <string>
#include <vector>
#include "boost/regex.hpp"

namespace ttp
{
enum STATE
{
    STATE_SUCCESS = 0,
    STATE_NETWORK_ERROR,
    STATE_DATA_ERROR,
    STATE_DATE_TIME_ERROR,
    STATE_SELECT_TRAIN_ERROR,
    STATE_START_ARRIVE_IS_SAME_ERROR,
    STATE_DATA_NOT_FOUND
};

#define CHECK_SUCCESS(x) ((x==STATE_SUCCESS) ? true : false)
#define IS_LEGAL_TIME(x) ((x.hour() >= 24 && x.minute() >= 0) ? false : true)

static std::string & trim(std::string & s)
{
    if (s.empty()) return s ;

    s.erase(0, s.find_first_not_of(" ")) ;
    s.erase(s.find_last_not_of(" ")+1) ;
    s.erase(0, s.find_first_not_of("\t")) ;
    s.erase(s.find_last_not_of("\t")+1) ;
    return s ;
}

/*
static std::vector<std::string> boost_split(std::string str, std::string symbol)
{
    boost::regex reg(symbol.c_str()) ;
    std::vector<std::string> vec ;
    boost::sregex_token_iterator it(str.begin(), str.end(), reg, -1) ;
    boost::sregex_token_iterator end ;
    while (it != end)
        vec.push_back(*it++);
    return vec ;
}
*/
}
#endif // STATE_H
