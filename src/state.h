/**
    Provide by Walon Li

    File: state.h
**/

#ifndef STATE_H
#define STATE_H
namespace ttp
{
enum STATE
{
    STATE_SUCCESS = 0,
    STATE_NETWORK_ERROR,
    STATE_ERROR
};

#define CHECK_SUCCESS(x) ((x==STATE_SUCCESS) ? true : false)
}
#endif // STATE_H
