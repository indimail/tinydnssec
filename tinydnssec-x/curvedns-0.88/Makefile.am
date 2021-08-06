# CurveDNS' Makefile template
#
# The @VAR@ tokens will be replaced by configure.curvedns
AUTOMAKE_OPTIONS = foreign no-dependencies

NACLINC=/usr/include/sodium
CDNSCFLAGS=-Wall -fno-strict-aliasing -O3 -I$(NACLINC)

DEFS=$(CFLAGS) -g -m64 -O3 -fomit-frame-pointer -funroll-loops $(CDNSCFLAGS) $(EVCFLAGS)

bin_PROGRAMS = curvedns
sbin_PROGRAMS = curvedns-keygen
sysconf_DATA = curvedns.private.key
man_MANS = curvedns.8 curvedns-keygen.8 curvedns-conf.8

debug.o: debug.c debug.h
	$(CC) $(DEFS) -c debug.c

cache_hashtable.o: cache_hashtable.c cache_hashtable.h debug.o
	$(CC) $(DEFS) -c cache_hashtable.c

# ready for possible critbit addition
cache.a: cache_hashtable.o
	$(AR) cr cache.a cache_hashtable.o
	ranlib cache.a

dns.o: dns.c dns.h debug.o event.a
	$(CC) $(DEFS) -c dns.c

dnscurve.o: dnscurve.c dnscurve.h debug.o event.a
	$(CC) $(DEFS) -c dnscurve.c

curvedns.o: curvedns.c curvedns.h debug.o ip.o misc.o
	$(CC) $(DEFS) -c curvedns.c

ip.o: ip.c ip.h debug.o
	$(CC) $(DEFS) -c ip.c

event_tcp.o: event_tcp.c event.h debug.o ip.o cache.a
	$(CC) $(DEFS) -c event_tcp.c

event_udp.o: event_udp.c event.h debug.o ip.o cache.a
	$(CC) $(DEFS) -c event_udp.c

event_main.o: event_main.c event.h debug.o ip.o cache.a
	$(CC) $(DEFS) -c event_main.c

event.a: event_main.o event_udp.o event_tcp.o
	$(AR) cr event.a event_main.o event_udp.o event_tcp.o
	ranlib event.a

misc.o: misc.c misc.h ip.o debug.o
	$(CC) $(DEFS) -c misc.c

curvedns-keygen.o: curvedns-keygen.c
	$(CC) $(DEFS) -c curvedns-keygen.c

# The targets:
curvedns$(EXEEXT): debug.o ip.o misc.o cache.a event.a dnscurve.o dns.o curvedns.o
	$(CC) -z noexecstack $(LDFLAGS) debug.o ip.o misc.o dnscurve.o dns.o cache.a event.a curvedns.o $(LIB_EV) $(LIB_SODIUM) -o curvedns

curvedns-keygen$(EXEEXT): curvedns-keygen.o debug.o ip.o misc.o
	$(CC) $(LDFLAGS) curvedns-keygen.o debug.o ip.o misc.o $(LIB_SODIUM) -o curvedns-keygen
