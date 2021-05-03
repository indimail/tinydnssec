#ifndef LOG_H
#define LOG_H

#include "uint64.h"

extern void log_startup(void);

extern void log_query(uint64 *,const char client[16],unsigned int,const char id[2],const char *,const char qtype[2]);
extern void log_querydrop(uint64 *);
extern void log_querydropmaxsoa(uint64 *);
extern void log_querydone(uint64 *,unsigned int);

extern void log_tcpopen(const char client[16],unsigned int);
extern void log_tcpclose(const char client[16],unsigned int);

extern void log_cachedanswer(const char *,const char type[2]);
extern void log_cachedcname(const char *,const char *);
extern void log_cachednxdomain(const char *);
extern void log_cachedns(const char *,const char *);

extern void log_tx(const char *,const char qtype[2],const char *,const char servers[256],unsigned int);

extern void log_nxdomain(const char server[16],const char *,unsigned int);
extern void log_nodata(const char server[16],const char *,const char qypte[2],unsigned int);
extern void log_servfail(const char *);
extern void log_lame(const char server[16],const char *,const char *);

extern void log_rr(const char server[16],const char *,const char type[2],const char *,unsigned int,unsigned int);
extern void log_rrns(const char server[16],const char *,const char *,unsigned int);
extern void log_rrcname(const char server[16],const char *,const char *,unsigned int);
extern void log_rrptr(const char server[16],const char *,const char *,unsigned int);
extern void log_rrmx(const char server[16], const char *,const char *,const char pref[2],unsigned int);
extern void log_rrsoa(const char server[16], const char *,const char *,const char *,const char misc[20],unsigned int);

extern void log_stats(void);
extern void log_qcount(int);

#endif
