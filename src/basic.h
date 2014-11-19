/**
    Provide by Walon Li

    File: state.h
**/

#ifndef STATE_H
#define STATE_H

#include <string>

namespace ttp
{
enum STATE
{
    STATE_SUCCESS = 0,
    STATE_NETWORK_ERROR,
    STATE_DATA_ERROR
};

#define CHECK_SUCCESS(x) ((x==STATE_SUCCESS) ? true : false)


static std::string & trim(std::string & s)
{
    if (s.empty()) return s ;

    s.erase(0, s.find_first_not_of(" ")) ;
    s.erase(s.find_last_not_of(" ")+1) ;
    return s ;
}

}
#endif // STATE_H
