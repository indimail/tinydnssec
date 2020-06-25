# tinydnssec

djbdns with dnssec, dnscurve support - a collection of Domain Name System tools

The internet has evolved a lot since djbdns was written, and dnscache does not handle todays (questionable) site setups spread over multiple domains very well. In most cases using a cache like unbound will perform significantly better.

If you still want to run dnscache follow the instructions on the djbdns
hompage: http://cr.yp.to/djbdns/run-cache.html

This package includes software for all the fundamental DNS operations - See https://blog.ploetzli.ch/2014/tinydns-dnssec/

DNS cache: finding addresses of Internet hosts.  When a browser wants to contact www.yahoo.com, it first asks a DNS cache, such as djbdns's dnscache, to find the IP address of www.yahoo.com.  Internet service providers run dnscache to find IP addresses requested by their customers. If you're running a home computer or a workstation, you can run your own dnscache to speed up your web browsing.

DNS server: publishing addresses of Internet hosts.  The IP address of www.yahoo.com is published by Yahoo's DNS servers.  djbdns includes a general-purpose DNS server, tinydns; network administrators run tinydns to publish the IP addresses of their computers.  djbdns also includes special-purpose servers for publishing DNS walls and RBLs.

DNS client: talking to a DNS cache.  djbdns includes a DNS client C library and several command-line DNS client utilities.  Programmers use these tools to send requests to DNS caches.

djbdns also includes several DNS debugging tools, notably dnstrace, which administrators use to diagnose misconfigured remote servers.

DQ: A package with DNS/DNSCurve related software. It contains a recursive DNS server with DNSCurve support called dqcache and also a commandline tool to debug DNS/DNScurve called dq.

See https://mojzis.com/software/dq/

CurveDNS: CurveDNS is the first publicly released forwarding implementation that implements the DNSCurve protocol.  DNSCurve uses high-speed high-security elliptic-curve cryptography to drastically improve every dimension of DNS security.

See http://dnscurve.org/ for protocol details.

curvedns allows any authoritative DNS name server to act as a DNSCurve capable one, without changing anything on your current DNS environment. The only thing a DNS data manager (that is probably you) has to do is to install CurveDNS on a machine, generate a keypair, and update NS type records that were pointing towards your authoritative name server and let them point to this machine running CurveDNS. Indeed, it is that easy to become fully protected against almost any of the currently known DNS flaws, such as active and passive cache poisoning.

CurveDNS supports:
 * Forwarding of regular (non-protected) DNS packets;
 * Unboxing of DNSCurve queries and forwarding the regular DNS packets
 * Boxing of regular DNS responses to DNSCurve responses;
 * Both DNSCurve's streamlined- and TXT-format;
 * Caching of shared secrets;
 * Both UDP and TCP;
 * Both IPv4 and IPv6.

# SUPPORT INFORMATION

tinydnssec is supported at IndiMail

## IRC / Matrix

* Join me [#indimail:matrix.org](https://matrix.to/#/#indimail:matrix.org)
* IndiMail has an IRC channel #indimail-mta

## Mailing list

There are four Mailing Lists for IndiMail

1. indimail-support  - You can subscribe for Support [here](https://lists.sourceforge.net/lists/listinfo/indimail-support). You can mail [indimail-support](mailto:indimail-support@lists.sourceforge.net) for support Old discussions can be seen [here](https://sourceforge.net/mailarchive/forum.php?forum_name=indimail-support)
2. indimail-devel - You can subscribe [here](https://lists.sourceforge.net/lists/listinfo/indimail-devel). You can mail [indimail-devel](mailto:indimail-devel@lists.sourceforge.net) for development activities. Old discussions can be seen [here](https://sourceforge.net/mailarchive/forum.php?forum_name=indimail-devel)
3. indimail-announce - This is only meant for announcement of New Releases or patches. You can subscribe [here](http://groups.google.com/group/indimail)
4. Archive at [Google Groups](http://groups.google.com/group/indimail). This groups acts as a remote archive for indimail-support and indimail-devel.

There is also a [Project Tracker](http://sourceforge.net/tracker/?group_id=230686) for IndiMail (Bugs, Feature Requests, Patches, Support Requests)
