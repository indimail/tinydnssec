# tinydnssec

djbdns with dnssec, dnscurve support - a collection of Domain Name System tools

**Complation Status (from Github Actions)**

[![tinydnssec Ubuntu CI](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-c-cpp.yml/badge.svg)](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-c-cpp.yml)
[![tinydnssec FreeBSD CI](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-freebsd.yml/badge.svg)](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-freebsd.yml)

The internet has evolved a lot since djbdns was written, and dnscache does not handle todays (questionable) site setups spread over multiple domains very well. In most cases using a cache like unbound will perform significantly better.

If you still want to run dnscache follow the instructions on the djbdns
hompage: http://cr.yp.to/djbdns/run-cache.html

This package includes software for all the fundamental DNS operations - See [Henryk Plötz's tinydnssec](https://blog.ploetzli.ch/2014/tinydns-dnssec/).

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

# Source Compiling/Linking

tinydnssec uses functions from [libqmail](https://github.com/mbhangui/libqmail) for standard io, string functions and other common tasks.

## Download / clone libqmail

libqmail uses GNU autotools. You need to have autoconf, automake, libtool and pkg config package. Follow the instructions below to have them installed in case you don't have them.

```
$ cd /usr/local/src
$ git clone https://github.com/mbhangui/libqmail.git
$ cd /usr/local/src/libqmail
$ ./default.configure
$ make
$ sudo make install-strip
```

(check version in libqmail/conf-version)

NOTE Darwin (Mac OSX) install [MacPorts](https://www.macports.org/) or brew. You can look at this [document](https://paolozaino.wordpress.com/2015/05/05/how-to-install-and-use-autotools-on-mac-os-x/) for installing MacPorts.

```
Darwin
# port install autoconf libtool automake pkgconfig libev
# port install openssl (# openssl is required for building libqmail)
# port update outdated
```

```
FreeBSD - install packages using pkg
# pkg install automake autoconf libtool pkgconf

Arch Linux
# pacman -S --refresh --sysupgrade
# pacman -S --needed archlinux-keyring
# pacman -S base-devel diffutils coreutils openssl

Gentoo Linux
# emaint -a sync
# emerge-webrsync
# emerge -a app-portage/eix
# eix-sync
# emerge -a dev-libs/libev
# etc-update
```

## Download tinydnssec

```
$ cd /usr/local/src
$ git clone https://github.com/mbhangui/tinydnssec.git
```

## Compile and install tinydnssec

To configure the build for tinydnssec, you need to configure conf-home, conf-sysconfdir and conf-servicedir. Defaults are given in the table below. If you are ok with defaults, you can run the script default.configure to set the below values.

**Linux**

config file|value
-----------|------
conf-home|/usr
conf-sysconfdir|/etc/indimail
conf-servicedir|/service

**FreeBSD**, **Darwin**

config file|value
-----------|------
conf-home|/usr/local
conf-sysconfdir|/usr/local/etc/indimail
conf-servicedir|/usr/local/etc/indimail/sv

The build below depends on several Makefile variables. For the build to operate without errors, you need to run default.configure the first time and everytime if you do a `make distclean`. If you don't run default.configure, you can replace `make` with `./qmake`

```
$ cd tinydnssec/tinydnssec-1.05
$ ./default.configure
$ make
$ sudo make install
```

## Compile and install dq

```
$ cd tinydnssec/tinydnssec-1.05/dq-20161210
$ make
$ sudo make install
```

### run dqcache

under root - create dqcache root directory

```
# mkdir -p /etc/indimail/dqcache/root/servers /etc/indimail/dqcache/env
# echo 10000000 > /etc/indimail/dqcache/env/CACHESIZE
# echo 127.0.0.1 > /etc/indimail/dqcache/env/IP
# echo "/etc/indimail/dqcache/root" > /etc/dqcache/env/ROOT
```

under root - setup dqcache root servers

```
# sh -c '(
echo "198.41.0.4"
echo "2001:503:ba3e::2:30"
echo "192.228.79.201"
echo "2001:500:84::b"
echo "192.33.4.12"
echo "2001:500:2::c"
echo "199.7.91.13"
echo "2001:500:2d::d"
echo "192.203.230.10"
echo "192.5.5.241"
echo "2001:500:2f::f"
echo "192.112.36.4"
echo "198.97.190.53"
echo "2001:500:1::53"
echo "192.36.148.17"
echo "2001:7fe::53"
echo "192.58.128.30"
echo "2001:503:c27::2:30"
echo "193.0.14.129"
echo "2001:7fd::1"
echo "199.7.83.42"
echo "2001:500:9f::42"
echo "202.12.27.33"
echo "2001:dc3::35"
) > /etc/dqcache/root/servers/@'
```

under root - create dqcache user

`# useradd dqcache`

under root - run dqcache server

```
# envuidgid dqcache envdir /etc/indimail/dqcache/env dqcache
```

---------------------------------------------------------

## Compile and install curvedns

```
$ cd curvedns-0.88
$ ./configure.nacl
$ ./configure.curvedns
$ make
$ sudo make install
```

NOTE: I have not been able to build curvedns on OSX. There is an issue with the nacl library build not having the full set of include files.

### Setting up curvedns

For help in installing and setting up curvedns look at this [Document](https://github.com/mbhangui/tinydnssec/blob/master/tinydnssec-1.05/curvedns-0.88/INSTALL-curvedns.md)

# Binary Builds on openSUSE Build Service

**[Build Status on](https://build.opensuse.org/project/monitor/home:mbhangui) [Open Build Service](https://build.opensuse.org/project/show/home:mbhangui)**

[![tinydnssec obs trigger](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-obs.yml/badge.svg)](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-obs.yml)

You can get binary RPM / Debian packages at

* [Stable Releases](http://download.opensuse.org/repositories/home:/indimail/)
* [Experimental Releases](http://download.opensuse.org/repositories/home:/mbhangui/)

If you want to use DNF / YUM / apt-get, the corresponding install instructions for the two repositories, depending on whether you want to install a stable or an experimental release, are

* [Stable](https://software.opensuse.org/download.html?project=home%3Aindimail&package=ezmlm-idx)
* [Experimental](https://software.opensuse.org/download.html?project=home%3Ambhangui&package=ezmlm-idx)

```
Currently, the list of supported distributions for tinydnssec is

    * Arch Linux

    * SUSE
          o openSUSE_Leap_15.2
          o openSUSE_Leap_15.3
          o openSUSE_Tumbleweed
          o SUSE Linux Enterprise 12
          o SUSE Linux Enterprise 12 SP1
          o SUSE Linux Enterprise 12 SP2
          o SUSE Linux Enterprise 12 SP3
          o SUSE Linux Enterprise 12 SP4
          o SUSE Linux Enterprise 12 SP5
          o SUSE Linux Enterprise 15
          o SUSE Linux Enterprise 15 SP1
          o SUSE Linux Enterprise 15 SP2
          o SUSE Linux Enterprise 15 SP3

    * Red Hat
          o Fedora 33
          o Fedora 34
          o Red Hat Enterprise Linux 7
          o Scientific Linux 7
          o CentOS 7
          o CentOS 8
          o CentOS 8 Stream

    * Debian
          o Debian  9.0
          o Debian 10.0
          o Univention_4.3
          o Univention_4.4

    * Ubuntu
          o Ubuntu 16.04
          o Ubuntu 17.04
          o Ubuntu 18.04
          o Ubuntu 19.04
          o Ubuntu 19.10
          o Ubuntu 20.04
          o Ubuntu 20.10
          o Ubuntu 21.04
```

NOTE: You can also build local binary packages. To generate RPM packages locally for all components refer to [Create Local Binary Packages](.github/CREATE-Packages.md)

# SUPPORT INFORMATION

tinydnssec is supported at IndiMail

## IRC / Matrix

![Matrix](https://img.shields.io/matrix/indimail:matrix.org)

* [Matrix Invite Link #indimail:matrix.org](https://matrix.to/#/#indimail:matrix.org)
* IndiMail has an [IRC channel on libera](https://libera.chat/) #indimail-mta

## Mailing list

There are four Mailing Lists for IndiMail

1. indimail-support  - You can subscribe for Support [here](https://lists.sourceforge.net/lists/listinfo/indimail-support). You can mail [indimail-support](mailto:indimail-support@lists.sourceforge.net) for support Old discussions can be seen [here](https://sourceforge.net/mailarchive/forum.php?forum_name=indimail-support)
2. indimail-devel - You can subscribe [here](https://lists.sourceforge.net/lists/listinfo/indimail-devel). You can mail [indimail-devel](mailto:indimail-devel@lists.sourceforge.net) for development activities. Old discussions can be seen [here](https://sourceforge.net/mailarchive/forum.php?forum_name=indimail-devel)
3. indimail-announce - This is only meant for announcement of New Releases or patches. You can subscribe [here](http://groups.google.com/group/indimail)
4. Archive at [Google Groups](http://groups.google.com/group/indimail). This groups acts as a remote archive for indimail-support and indimail-devel.

There is also a [Project Tracker](http://sourceforge.net/tracker/?group_id=230686) for IndiMail (Bugs, Feature Requests, Patches, Support Requests)
