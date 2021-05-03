#ifndef QLOG_H
#define QLOG_H

#include "uint16.h"

extern void qlog(const char ip[16],uint16 port,const char id[2],const char *q,const char qtype[2],const char *result);

#endif
