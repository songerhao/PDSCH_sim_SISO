#ifndef _TIME
#define _TIME
#include <time.h>
#include "Constant.h"
#define START(name) echo_runtime(RUN,name)
#define TERMINATE echo_runtime(END)
void echo_runtime(int signal,char* name=NULL);
void push();
void pop();
#endif
