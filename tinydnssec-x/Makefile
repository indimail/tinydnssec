# Don't edit Makefile! Use conf-* for configuration.

SHELL=/bin/sh
include Makevars.$(SYSTEM)
curvedns_version=0.88

edit = sed \
	-e 's,@prefix\@,$(prefix),g' \
	-e 's}@version\@}$(version)}g' \
	-e 's|@release\@|$(release)|g' \
	-e 's,@libexecdir\@,$(libexecdir),g' \
	-e 's,@sysconfdir\@,$(sysconfdir),g' \
	-e 's,@servicedir\@,$(servicedir),g' \
	-e 's|@email\@|$(email)|g' \
	-e 's,@HOST\@,$(HOST),g' \
	-e 's,@DATE\@,$(DATE),g'

default: it

install-strip: install

auto-str: \
load auto-str.o
	./load auto-str -lqmail

auto-str.o: \
compile auto-str.c
	./compile auto-str.c

auto_home.c: \
auto-str conf-home
	./auto-str auto_home `head -1 conf-home` > auto_home.c

auto_home.o: \
compile auto_home.c
	./compile auto_home.c

auto_sysconfdir.c: \
auto-str conf-sysconfdir
	./auto-str auto_sysconfdir `head -1 conf-sysconfdir` > auto_sysconfdir.c

auto_sysconfdir.o: \
compile auto_sysconfdir.c
	./compile auto_sysconfdir.c

axfr-get: load axfr-get.o iopause.o \
ip4_fmt.o ip6_fmt.o dns.a
	./load axfr-get iopause.o \
	ip4_fmt.o ip6_fmt.o dns.a -lqmail

axfr-get.o: \
compile axfr-get.c dns.h iopause.h
	./compile axfr-get.c

axfrdns: \
load axfrdns.o iopause.o droproot.o tdlookup.o response.o qlog.o \
prot.o clientloc.o dns.a sha1.o base32hex.o edns0.o ip4_scan.o \
ip6_scan.o ip4_fmt.o ip6_fmt.o
	./load axfrdns iopause.o droproot.o tdlookup.o response.o \
	qlog.o prot.o clientloc.o dns.a sha1.o base32hex.o edns0.o \
	ip4_scan.o ip6_scan.o ip4_fmt.o ip6_fmt.o -lqmail

axfrdns-conf: \
load axfrdns-conf.o generic-conf.o auto_home.o
	./load axfrdns-conf generic-conf.o auto_home.o -lqmail

axfrdns-conf.o: \
compile axfrdns-conf.c auto_home.h generic-conf.h
	./compile axfrdns-conf.c

axfrdns.o: \
compile axfrdns.c droproot.h ip4.h iopause.h \
dns.h qlog.h response.h clientloc.h conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` axfrdns.c

base32hex.o: \
compile base32hex.c base32hex.h conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` base32hex.c

cache.o: \
compile cache.c cache.h dns.h iopause.h siphash.h
	./compile cache.c

siphash.o: \
compile siphash.c
	./compile siphash.c

cachetest: \
load cachetest.o siphash.o cache.o dns_random.o
	./load cachetest siphash.o cache.o dns_random.o -lqmail

cachetest.o: compile cachetest.c cache.h
	./compile cachetest.c

chkshsgr: \
load chkshsgr.o
	./load chkshsgr

chkshsgr.o: \
compile chkshsgr.c
	./compile chkshsgr.c

choose: \
warn-auto.sh choose.sh conf-home
	cat warn-auto.sh choose.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> choose
	chmod 755 choose

clientloc.o: \
compile clientloc.c ip6.h
	./compile clientloc.c

compile: \
warn-auto.sh conf-cc make-compile
	( cat warn-auto.sh; ./make-compile) > compile
	chmod 755 compile

make-compile: \
make-compile.sh
	( cat warn-auto.sh make-compile.sh) > make-compile
	chmod 755 make-compile

dd.o: \
compile dd.c dns.h iopause.h dd.h
	./compile dd.c

dns.a: \
makelib dns_dfd.o dns_domain.o dns_dtda.o dns_ip.o dns_ipq.o dns_mx.o \
dns_name.o dns_nd.o dns_packet.o dns_random.o dns_rcip.o dns_rcrw.o \
dns_resolve.o dns_sortip.o dns_transmit.o dns_txt.o dns_ip6.o \
dns_sortip6.o dns_nd6.o dns_ipq6.o
	./makelib dns.a dns_dfd.o dns_domain.o dns_dtda.o dns_ip.o \
	dns_ipq.o dns_mx.o dns_name.o dns_nd.o dns_packet.o \
	dns_random.o dns_rcip.o dns_rcrw.o dns_resolve.o \
	dns_sortip.o dns_transmit.o dns_txt.o dns_ip6.o dns_sortip6.o \
	dns_nd6.o dns_ipq6.o

dns_dfd.o: \
compile dns_dfd.c dns.h iopause.h
	./compile dns_dfd.c

dns_domain.o: \
compile dns_domain.c dns.h iopause.h
	./compile dns_domain.c

dns_dtda.o: \
compile dns_dtda.c dns.h iopause.h
	./compile dns_dtda.c

dns_ip.o: \
compile dns_ip.c dns.h iopause.h 
	./compile dns_ip.c

dns_ip6.o: \
compile dns_ip6.c dns.h iopause.h
	./compile dns_ip6.c

dns_ipq.o: \
compile dns_ipq.c dns.h iopause.h
	./compile dns_ipq.c

dns_ipq6.o: \
compile dns_ipq6.c dns.h iopause.h
	./compile dns_ipq6.c

dns_mx.o: \
compile dns_mx.c dns.h iopause.h
	./compile dns_mx.c

dns_name.o: \
compile dns_name.c dns.h iopause.h
	./compile dns_name.c

dns_nd.o: \
compile dns_nd.c dns.h iopause.h
	./compile dns_nd.c

dns_nd6.o: \
compile dns_nd6.c dns.h iopause.h
	./compile dns_nd6.c

dns_packet.o: \
compile dns_packet.c dns.h iopause.h
	./compile dns_packet.c

dns_random.o: \
compile dns_random.c dns.h iopause.h
	./compile dns_random.c

dns_rcip.o: \
compile dns_rcip.c ip4.h dns.h iopause.h
	./compile dns_rcip.c

dns_rcrw.o: \
compile dns_rcrw.c dns.h iopause.h
	./compile dns_rcrw.c

dns_resolve.o: \
compile dns_resolve.c iopause.h dns.h iopause.h
	./compile dns_resolve.c

dns_sortip.o: \
compile dns_sortip.c dns.h iopause.h
	./compile dns_sortip.c

dns_sortip6.o: \
compile dns_sortip6.c dns.h iopause.h
	./compile dns_sortip6.c

dns_transmit.o: \
compile dns_transmit.c socket.h dns.h iopause.h
	./compile dns_transmit.c

dns_txt.o: \
compile dns_txt.c dns.h iopause.h
	./compile dns_txt.c

dnscache: \
load dnscache.o droproot.o okclient.o log.o siphash.o \
cache.o dns_random.o query.o response.o dd.o roots.o \
iopause.o prot.o dns.a socket.a socket.lib
	./load dnscache droproot.o okclient.o log.o \
	siphash.o cache.o dns_random.o query.o response.o \
	dd.o roots.o iopause.o prot.o dns.a \
	socket.a `cat socket.lib` -lqmail

dnscache-conf: \
load dnscache-conf.o generic-conf.o auto_home.o \
auto_sysconfdir.o
	./load dnscache-conf generic-conf.o auto_home.o \
	auto_sysconfdir.o -lqmail

dnscache-conf.o: \
compile dnscache-conf.c hasdevtcp.h auto_home.h \
generic-conf.h auto_sysconfdir.h
	./compile dnscache-conf.c

dnscache.o: \
compile dnscache.c ip4.h socket.h dns.h \
roots.h iopause.h query.h response.h cache.h \
log.h okclient.h droproot.h
	./compile dnscache.c

dnsfilter: \
load dnsfilter.o iopause.o dns.a socket.a socket.lib
	./load dnsfilter iopause.o dns.a \
	socket.a `cat socket.lib` -lqmail

dnsfilter.o: \
compile dnsfilter.c dns.h iopause.h ip4.h iopause.h
	./compile dnsfilter.c

dnsip: load dnsip.o iopause.o dns.a socket.a socket.lib
	./load dnsip iopause.o dns.a \
	socket.a `cat socket.lib` -lqmail

dnsip.o: \
compile dnsip.c ip4.h dns.h iopause.h
	./compile dnsip.c

dnsip6: \
load dnsip6.o iopause.o dns.a socket.a socket.lib
	./load dnsip6 iopause.o dns.a \
	socket.a `cat socket.lib` -lqmail

dnsip6.o: \
compile dnsip6.c ip6.h dns.h iopause.h
	./compile dnsip6.c

dnsipq: \
load dnsipq.o iopause.o dns.a socket.a socket.lib
	./load dnsipq iopause.o dns.a \
	socket.a `cat socket.lib` -lqmail

dnsipq.o: \
compile dnsipq.c ip4.h dns.h iopause.h
	./compile dnsipq.c

dnsip6q: \
load dnsip6q.o iopause.o dns.a socket.a socket.lib
	./load dnsip6q iopause.o dns.a \
	socket.a `cat socket.lib` -lqmail

dnsip6q.o: \
compile dnsip6q.c ip4.h dns.h iopause.h
	./compile dnsip6q.c

dnsmx: \
load dnsmx.o iopause.o dns.a socket.a socket.lib
	./load dnsmx iopause.o dns.a \
	socket.a `cat socket.lib` -lqmail

dnsmx.o: \
compile dnsmx.c dns.h iopause.h
	./compile dnsmx.c

dnsname: \
load dnsname.o iopause.o dns.a socket.a socket.lib
	./load dnsname iopause.o dns.a \
	socket.a `cat socket.lib` -lqmail

dnsname.o: \
compile dnsname.c ip4.h dns.h iopause.h ip6.h
	./compile dnsname.c

dns_namex.o: \
compile dns_namex.c dns.h \
iopause.h
	./compile dns_namex.c

dnsnamex: \
load dnsnamex.o dns_namex.o iopause.o dns.a \
socket.a socket.lib
	./load dnsnamex dns_namex.o iopause.o dns.a \
	socket.a `cat socket.lib` -lqmail

dnsnamex.o: \
compile dnsnamex.c ip4.h dns.h \
iopause.h
	./compile dnsnamex.c

dnsq: \
load dnsq.o iopause.o printrecord.o printpacket.o parsetype.o dns.a \
socket.a socket.lib printtype.o base32hex.o
	./load dnsq iopause.o printrecord.o printpacket.o \
	parsetype.o dns.a socket.a `cat socket.lib` \
	printtype.o base32hex.o -lqmail

dnsq.o: \
compile dnsq.c \
ip4.h iopause.h printpacket.h parsetype.h dns.h iopause.h
	./compile dnsq.c

dnsqr: \
load dnsqr.o iopause.o printrecord.o printpacket.o parsetype.o dns.a \
socket.a socket.lib printtype.o base32hex.o
	./load dnsqr iopause.o printrecord.o printpacket.o \
	parsetype.o dns.a socket.a `cat socket.lib` \
	printtype.o base32hex.o -lqmail

dnsqr.o: \
compile dnsqr.c iopause.h printpacket.h parsetype.h dns.h iopause.h 
	./compile dnsqr.c

dnstrace: \
load dnstrace.o dd.o iopause.o printrecord.o parsetype.o dns.a \
socket.a socket.lib printtype.o base32hex.o
	./load dnstrace dd.o iopause.o printrecord.o parsetype.o \
	dns.a socket.a `cat socket.lib` printtype.o base32hex.o -lqmail

dnstrace.o: \
compile dnstrace.c ip4.h iopause.h printrecord.h parsetype.h dd.h dns.h iopause.h
	./compile dnstrace.c

dnstracesort: \
warn-auto.sh dnstracesort.sh conf-home
	cat warn-auto.sh dnstracesort.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> dnstracesort
	chmod 755 dnstracesort

dnsgetroot: \
warn-auto.sh dnsgetroot.in conf-home
	cat warn-auto.sh dnsgetroot.in \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> dnsgetroot
	chmod 755 dnsgetroot

dnstxt: load dnstxt.o iopause.o dns.a socket.a socket.lib
	./load dnstxt iopause.o dns.a socket.a \
	`cat socket.lib` -lqmail

dnstxt.o: \
compile dnstxt.c dns.h iopause.h
	./compile dnstxt.c

droproot.o: \
compile droproot.c prot.h
	./compile droproot.c

edns0.o: \
compile edns0.c edns0.h dns.h response.h iopause.h \
conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` edns0.c

generic-conf.o: \
compile generic-conf.c generic-conf.h
	./compile generic-conf.c

hasdevtcp.h: \
systype hasdevtcp.h1 hasdevtcp.h2
	( case "`cat systype`" in \
	  sunos-5.*) cat hasdevtcp.h2 ;; \
	  *) cat hasdevtcp.h1 ;; \
	esac ) > hasdevtcp.h

hasshsgr.h: \
choose compile load tryshsgr.c hasshsgr.h1 hasshsgr.h2 chkshsgr \
warn-shsgr
	./chkshsgr || ( cat warn-shsgr; exit 1 )
	./choose clr tryshsgr hasshsgr.h1 hasshsgr.h2 > hasshsgr.h

iopause.h: \
choose compile load trypoll.c iopause.h1 iopause.h2
	./choose clr trypoll iopause.h1 iopause.h2 > iopause.h

iopause.o: \
compile iopause.c select.h iopause.h 
	./compile iopause.c

ip4_fmt.o: \
compile ip4_fmt.c ip4.h
	./compile ip4_fmt.c

ip6_fmt.o: \
compile ip6_fmt.c ip6.h
	./compile ip6_fmt.c

ip4_scan.o: \
compile ip4_scan.c ip4.h
	./compile ip4_scan.c

ip6_scan.o: \
compile ip6_scan.c ip6.h
	./compile ip6_scan.c

it: \
prog instcheck tinydnssec.spec tinydnssec.changes PKGBUILD \
dnssvc installer

load: warn-auto.sh conf-ld systype make-load
	( cat warn-auto.sh; ./make-load "`cat systype`" ) > load
	chmod 755 load

make-load: \
make-load.sh auto-ccld.sh
	cat auto-ccld.sh make-load.sh > make-load
	chmod 755 make-load

auto-ccld.sh: conf-cc conf-ld warn-auto.sh
	(cat warn-auto.sh; echo CC=\'`head -1 conf-cc`\'; \
	echo LD=\'`head -1 conf-ld`\'; \
	) > auto-ccld.sh

log.o: \
compile log.c log.h
	./compile log.c

makelib: \
warn-auto.sh systype
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo 'rm -f "$$main"'; \
	echo 'ar cr "$$main" $${1+"$$@"}'; \
	case "`cat systype`" in \
	sunos-5.*) ;; \
	unix_sv*) ;; \
	irix64-*) ;; \
	irix-*) ;; \
	dgux-*) ;; \
	hp-ux-*) ;; \
	sco*) ;; \
	*) echo 'ranlib "$$main"' ;; \
	esac \
	) > makelib
	chmod 755 makelib

okclient.o: \
compile okclient.c ip4.h okclient.h
	./compile okclient.c

parsetype.o: \
compile parsetype.c dns.h iopause.h parsetype.h conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` parsetype.c

pickdns: \
load pickdns.o server.o iopause.o response.o droproot.o qlog.o prot.o dns.a \
socket.a socket.lib edns0.o
	./load pickdns server.o iopause.o response.o droproot.o qlog.o \
	prot.o dns.a socket.a `cat socket.lib` edns0.o -lqmail

pickdns-conf: \
load pickdns-conf.o generic-conf.o auto_home.o
	./load pickdns-conf generic-conf.o auto_home.o -lqmail

pickdns-conf.o: \
compile pickdns-conf.c auto_home.h generic-conf.h
	./compile pickdns-conf.c

pickdns-data: load pickdns-data.o ip4_scan.o ip6_scan.o dns.a
	./load pickdns-data dns.a ip4_scan.o ip6_scan.o -lqmail

pickdns-data.o: \
compile pickdns-data.c ip4.h dns.h iopause.h
	./compile pickdns-data.c

pickdns.o: \
compile pickdns.c dns.h iopause.h response.h
	./compile pickdns.c

printpacket.o: \
compile printpacket.c dns.h iopause.h printrecord.h printpacket.h printtype.h \
conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` printpacket.c

printrecord.o: \
compile printrecord.c dns.h iopause.h printrecord.h printtype.h conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` printrecord.c

printtype.o: \
compile printtype.c printtype.h dns.h iopause.h conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` printtype.c

prog: \
dnscache-conf dnscache walldns-conf walldns rbldns-conf rbldns \
rbldns-data pickdns-conf pickdns pickdns-data tinydns-conf tinydns \
tinydns-data tinydns-get tinydns-edit axfr-get axfrdns-conf axfrdns \
dnsip dnsipq dnsname dnstxt dnsmx dnsfilter random-ip dnsqr dnsq \
dnstrace dnstracesort cachetest utime rts dnsip6 dnsip6q dnsnamex \
dns-notify dnsgetroot dqcache-conf curvedns-conf

prot.o: \
compile prot.c hasshsgr.h prot.h
	./compile prot.c

qlog.o: \
compile qlog.c qlog.h
	./compile qlog.c

query.o: \
compile query.c roots.h log.h cache.h \
dns.h iopause.h dd.h response.h query.h
	./compile query.c

random-ip: load random-ip.o dns.a
	./load random-ip dns.a -lqmail

random-ip.o: \
compile random-ip.c dns.h iopause.h
	./compile random-ip.c

rbldns: load rbldns.o server.o iopause.o response.o dd.o \
droproot.o qlog.o prot.o dns.a socket.a socket.lib edns0.o
	./load rbldns server.o iopause.o response.o dd.o \
	droproot.o qlog.o prot.o socket.a dns.a \
	`cat socket.lib` edns0.o -lqmail

rbldns-conf: \
load rbldns-conf.o generic-conf.o auto_home.o
	./load rbldns-conf generic-conf.o auto_home.o -lqmail

rbldns-conf.o: \
compile rbldns-conf.c auto_home.h generic-conf.h
	./compile rbldns-conf.c

rbldns-data: \
load rbldns-data.o ip4_scan.o ip6_scan.o
	./load rbldns-data ip4_scan.o ip6_scan.o -lqmail

rbldns-data.o: \
compile rbldns-data.c ip4.h
	./compile rbldns-data.c

rbldns.o: \
compile rbldns.c ip4.h dns.h iopause.h dd.h response.h
	./compile rbldns.c

response.o: \
compile response.c dns.h iopause.h response.h conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` response.c

roots.o: \
compile roots.c ip4.h \
dns.h iopause.h roots.h
	./compile roots.c

rts: \
warn-auto.sh rts.sh conf-home
	cat warn-auto.sh rts.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> rts
	chmod 755 rts

select.h: \
choose compile trysysel.c select.h1 select.h2
	./choose c trysysel select.h1 select.h2 > select.h

server.o: \
compile server.c ip4.h socket.h droproot.h qlog.h \
response.h dns.h iopause.h edns0.h conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` server.c

instcheck: \
warn-auto.sh instcheck.in conf-libexec conf-sysconfdir
	cat warn-auto.sh instcheck.in | $(edit) > $@
	chmod +x $@

installer: \
load installer.o
	./load installer -lqmail

installer.o: \
compile installer.c
	./compile installer.c

INPUT: INPUT.in \
conf-home conf-libexec conf-sysconfdir
	$(edit) INPUT.in > $@

DIRS: DIRS.in \
conf-home conf-sysconfdir
	([ -f DIRS.$(SYSTEM) ] && \
	$(edit) DIRS.in DIRS.$(SYSTEM) || $(edit) DIRS.in) > DIRS

install: it installer DIRS conf-home conf-sysconfdir conf-libexec INPUT \
BIN SBIN LIBEXEC ETC MAN
	./installer $(DESTDIR)                         < DIRS
	./installer $(DESTDIR)$(prefix)                < BIN
	./installer $(DESTDIR)$(prefix)                < SBIN
	./installer $(DESTDIR)$(libexecdir)            < LIBEXEC
	./installer $(DESTDIR)$(sysconfdir)            < ETC
	./installer $(DESTDIR)$(prefix)/share/man      < MAN
	grep -E "BIN|ETC|MAN" ETC.extra | ./installer $(DESTDIR)$(sysconfdir)
	grep "\-conf" SBIN.extra      | ./installer $(DESTDIR)$(prefix)
	[ -z $(DESTDIR) ] && ./instcheck || true

socket.lib: \
trylsock.c compile load
	( ( ./compile trylsock.c && \
	./load trylsock -lsocket -lnsl ) >/dev/null 2>&1 \
	&& echo -lsocket -lnsl || exit 0 ) > socket.lib
	rm -f trylsock.o trylsock

socket_accept.o: \
compile socket_accept.c socket.h
	./compile socket_accept.c

socket_accept6.o: \
compile socket_accept6.c socket.h haveip6.h
	./compile socket_accept6.c

socket_bind.o: \
compile socket_bind.c socket.h
	./compile socket_bind.c

socket_bind6.o: \
compile socket_bind6.c sockaddr_in6.h haveip6.h socket.h ip6.h
	./compile socket_bind6.c

socket_conn.o: \
compile socket_conn.c socket.h
	./compile socket_conn.c

socket_connect6.o: \
compile socket_connect6.c socket.h haveip6.h
	./compile socket_connect6.c

socket_listen.o: \
compile socket_listen.c socket.h
	./compile socket_listen.c

socket_recv.o: \
compile socket_recv.c socket.h
	./compile socket_recv.c

socket_recv6.o: \
compile socket_recv6.c sockaddr_in6.h haveip6.h socket.h ip6.h
	./compile socket_recv6.c

socket_send.o: \
compile socket_send.c socket.h
	./compile socket_send.c

socket_send6.o: \
compile socket_send6.c socket.h ip6.h haveip6.h
	./compile socket_send6.c

socket_tcp.o: \
compile socket_tcp.c socket.h
	./compile socket_tcp.c

socket_tcp6.o: \
compile socket_tcp6.c socket.h haveip6.h
	./compile socket_tcp6.c

socket_udp.o: \
compile socket_udp.c socket.h
	./compile socket_udp.c

socket_udp6.o: \
compile socket_udp6.c socket.h haveip6.h
	./compile socket_udp6.c

socket_noipv6.o: \
compile socket_noipv6.c haveip6.h
	./compile socket_noipv6.c

socket_getifidx.o: \
compile socket_getifidx.c socket.h haven2i.h
	./compile socket_getifidx.c

haven2i.h: \
tryn2i.c choose compile load socket.lib haven2i.h1 haven2i.h2
	cp /dev/null haven2i.h
	./choose cL tryn2i haven2i.h1 haven2i.h2 socket > haven2i.h

sha1.o: \
compile sha1.c sha1.h conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` sha1.c

systype: \
find-systype.sh conf-cc conf-ld trycpp.c x86cpuid.c
	( cat warn-auto.sh; \
	echo CC=\'`head -1 conf-cc`\'; \
	echo LD=\'`head -1 conf-ld`\'; \
	cat find-systype.sh; \
	) | sh > systype

tdlookup.o: \
compile tdlookup.c dns.h iopause.h response.h ip6.h \
clientloc.h sha1.h base32hex.h conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` tdlookup.c

tinydns: \
load tinydns.o server.o iopause.o droproot.o tdlookup.o response.o qlog.o \
prot.o clientloc.o dns.a socket.a socket.lib sha1.o base32hex.o edns0.o
	./load tinydns server.o iopause.o droproot.o tdlookup.o response.o \
	qlog.o prot.o clientloc.o dns.a socket.a `cat socket.lib` sha1.o \
	base32hex.o edns0.o -lqmail

tinydns-conf: \
load tinydns-conf.o generic-conf.o auto_home.o \
socket.a
	./load tinydns-conf generic-conf.o auto_home.o \
	socket.a -lqmail

tinydns-conf.o: \
compile tinydns-conf.c auto_home.h generic-conf.h
	./compile tinydns-conf.c

tinydns-data: \
load tinydns-data.o ip4_scan.o ip6_scan.o dns.a
	./load tinydns-data dns.a ip4_scan.o ip6_scan.o -lqmail

tinydns-data.o: \
compile tinydns-data.c ip4.h dns.h iopause.h ip6.h conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` tinydns-data.c

tinydns-edit: load tinydns-edit.o dns.a ip4_fmt.o ip6_fmt.o \
ip4_scan.o ip6_scan.o
	./load tinydns-edit dns.a ip4_fmt.o ip6_fmt.o \
	ip4_scan.o ip6_scan.o -lqmail

tinydns-edit.o: \
compile tinydns-edit.c ip4.h dns.h iopause.h
	./compile tinydns-edit.c

tinydns-get: \
load tinydns-get.o tdlookup.o sha1.o response.o \
printpacket.o printrecord.o parsetype.o clientloc.o \
dns.a base32hex.o printtype.o ip4_fmt.o ip6_fmt.o \
ip4_scan.o ip6_scan.o
	./load tinydns-get tdlookup.o response.o printpacket.o \
	printrecord.o parsetype.o clientloc.o dns.a sha1.o \
	base32hex.o printtype.o ip4_fmt.o ip6_fmt.o \
	ip4_scan.o ip6_scan.o -lqmail

tinydns-get.o: \
compile tinydns-get.c response.h printpacket.h \
parsetype.h ip4.h dns.h iopause.h conf-dnssec
	./compile `grep -h -v "^#" conf-dnssec` tinydns-get.c

tinydns.o: \
compile tinydns.c dns.h iopause.h
	./compile tinydns.c

socket.a: \
makelib socket_accept.o socket_bind.o socket_conn.o \
socket_listen.o socket_recv.o socket_send.o socket_tcp.o \
socket_udp.o socket_udp6.o socket_getifidx.o socket_recv6.o \
socket_send6.o socket_bind6.o socket_noipv6.o socket_tcp6.o \
socket_connect6.o socket_accept6.o \
ip4_fmt.o ip6_fmt.o ip4_scan.o ip6_scan.o
	./makelib socket.a socket_accept.o socket_bind.o \
	socket_conn.o socket_listen.o socket_recv.o socket_send.o \
	socket_tcp.o socket_udp.o socket_udp6.o socket_getifidx.o \
	socket_recv6.o socket_send6.o socket_bind6.o socket_noipv6.o \
	socket_tcp6.o socket_connect6.o socket_accept6.o \
	ip4_fmt.o ip6_fmt.o ip4_scan.o ip6_scan.o

utime: load utime.o 
	./load utime -lqmail

utime.o: \
compile utime.c
	./compile utime.c

walldns: \
load walldns.o server.o iopause.o response.o droproot.o qlog.o prot.o dd.o \
dns.a socket.lib edns0.o socket.a
	./load walldns server.o iopause.o response.o droproot.o qlog.o \
	prot.o dd.o dns.a socket.a `cat socket.lib` edns0.o -lqmail

walldns-conf: \
load walldns-conf.o generic-conf.o auto_home.o
	./load walldns-conf generic-conf.o auto_home.o -lqmail

walldns-conf.o: \
compile walldns-conf.c auto_home.h generic-conf.h
	./compile walldns-conf.c

dqcache-conf: \
load dqcache-conf.o generic-conf.o auto_home.o \
auto_sysconfdir.o 
	./load dqcache-conf generic-conf.o auto_home.o \
	auto_sysconfdir.o -lqmail

dqcache-conf.o: \
compile dqcache-conf.c auto_home.h generic-conf.h \
auto_sysconfdir.h
	./compile dqcache-conf.c

curvedns-conf: \
load curvedns-conf.o generic-conf.o auto_home.o \
auto_sysconfdir.o
	./load curvedns-conf generic-conf.o auto_home.o \
	auto_sysconfdir.o -lqmail

curvedns-conf.o: \
compile curvedns-conf.c auto_home.h generic-conf.h \
auto_sysconfdir.h
	./compile curvedns-conf.c

walldns.o: \
compile walldns.c dns.h iopause.h dd.h response.h
	./compile walldns.c

haveip6.h: \
tryip6.c choose compile haveip6.h1 haveip6.h2
	./choose c tryip6 haveip6.h1 haveip6.h2 > haveip6.h

sockaddr_in6.h: \
trysa6.c choose compile sockaddr_in6.h1 sockaddr_in6.h2 haveip6.h
	./choose c trysa6 sockaddr_in6.h1 sockaddr_in6.h2 > sockaddr_in6.h

dns-notify.o: \
compile dns-notify.c dns-notify.h
	./compile dns-notify.c

dns-notify: \
load dns-notify.o
	./load dns-notify -lqmail

tinydnssec.spec: tinydnssec.spec.in catChangeLog doc/ChangeLog \
conf-version conf-release conf-email
	(cat $@.in;./catChangeLog) | $(edit) > $@
tinydnssec.changes: doc/ChangeLog conf-version conf-release conf-email
	./catChangeLog --changes doc/ChangeLog > $@
PKGBUILD: PKGBUILD.in conf-version conf-release doc/ChangeLog \
conf-email tinydnssec.changes
	cat $@.in | $(edit) > PKGBUILD

dnssvc: dnssvc.in conf-version conf-release conf-email conf-home \
conf-servicedir conf-sysconfdir warn-auto.sh
	$(edit) warn-auto.sh $@.in > dnssvc
	chmod 755 dnssvc

clean:
	rm -f `cat TARGETS` data data.cdb test/[ot]*
	cd dq-20161210 && make clean && cd ..
	if test -f curvedns-$(curvedns_version)/Makefile ; then \
		cd curvedns-$(curvedns_version) && make clean; \
		make clean; \
	fi

distclean: clean
	rm -f `cat TARGETS` data data.cdb test/[ot]*
