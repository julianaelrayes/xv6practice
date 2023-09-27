#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

struct pstat {
    int pid;
    int create_time;
    int stop_time;
    int run_time;
    int numChosen;
};


#endif // _PSTAT_H_