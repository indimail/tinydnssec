[![Matrix](https://img.shields.io/matrix/indimail:matrix.org.svg)](https://matrix.to/#/#indimail:matrix.org)

**Compilation Status (from Github Actions)**

[![tinydnssec Ubuntu CI](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-c-cpp.yml/badge.svg)](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-c-cpp.yml)
[![tinydnssec FreeBSD CI](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-freebsd.yml/badge.svg)](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-freebsd.yml)

# tinydnssec

djbdns with dnssec, dnscurve support - a collection of Domain Name System tools

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
# port install autoconf libtool automake pkgconfig libev libsodium
# port install openssl (# openssl is required for building libqmail)
# port update outdated
```

```
FreeBSD - install packages using pkg
# pkg install automake autoconf libtool pkgconf

Arch Linux
# pacman -S --refresh --sysupgrade
# pacman -S --needed archlinux-keyring
# pacman -S base-devel diffutils coreutils openssl libev libsodium

Gentoo Linux
# emaint -a sync
# emerge-webrsync
# emerge -a app-portage/eix
# eix-sync
# emerge -a dev-libs/libev
# emerge -a dev-libs/libsodium
# etc-update

alpine Linux
# apk add gcc g++ make git autoconf automake libtool m4 sed
# apk add libev-dev openssl-dev libsodium-dev
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

Curvedns needs nacl library. It can be built with the nacl library in the `nacl` subdirectory in curvedns-0.88 directory or you can install libsodium. Instructions for installing libsodium have already been provided above.

```
$ cd curvedns-0.88

If you have libsodium installed then you can do the below. This option is fast.
$ ./default.configure
$ make
$ sudo make install-strip

If you want to use the provided nacl library present in nacl subdirectory.
$ ./configure.nacl     # this will build the nacl library and takes time
$ ./configure.curvedns # this command will create a Makefile
$ make
$ sudo make install-strip
```

NOTE: To build curvedns on OSX you have to use libsodium

### Setting up curvedns

For help in installing and setting up curvedns look at this [Document](https://github.com/mbhangui/tinydnssec/blob/master/tinydnssec-1.05/curvedns-0.88/INSTALL-curvedns.md)

# Binary Builds on openSUSE Build Service and Copr

**[Build Status on](https://build.opensuse.org/project/monitor/home:mbhangui) [Open Build Service](https://build.opensuse.org/project/show/home:mbhangui)**

[![tinydnssec obs trigger](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-obs.yml/badge.svg)](https://github.com/mbhangui/tinydnssec/actions/workflows/tinydnssec-obs.yml)

[![tinydnssec](https://build.opensuse.org/projects/home:mbhangui/packages/tinydnssec/badge.svg?type=percent)](https://build.opensuse.org/package/show/home:mbhangui/tinydnssec)

You can get binary RPM / Debian packages at

* [Stable Releases](http://download.opensuse.org/repositories/home:/indimail/)
* [Experimental Releases](http://download.opensuse.org/repositories/home:/mbhangui/)
* [copr Releases](https://copr.fedorainfracloud.org/coprs/cprogrammer/indimail)

If you want to use DNF / YUM / apt-get, the corresponding install instructions for the two repositories, depending on whether you want to install a stable or an experimental release, are

* [Stable](https://software.opensuse.org/download.html?project=home%3Aindimail&package=tinydnssec)
* [Experimental](https://software.opensuse.org/download.html?project=home%3Ambhangui&package=tinydnssec)
* [copr Releases](https://copr.fedorainfracloud.org/coprs/cprogrammer/indimail). The copr repository can be enabled by running the command 
  `$ sudo dnf copr enable cprogrammer/indimail`

The binary build sets up supervised service directories for dnscache, tinydns, dqcache, and curvedns in /etc/indimail. You can create a link to the directory in /service to run them as supervised services under svscan.

**[Build Status on](https://copr.fedorainfracloud.org/coprs/cprogrammer/indimail/monitor/) [copr](https://copr.fedorainfracloud.org/coprs/)**

[![tinydnssec](https://copr.fedorainfracloud.org/coprs/cprogrammer/indimail/package/tinydnssec/status_image/last_build.png)](https://copr.fedorainfracloud.org/coprs/cprogrammer/indimail/package/tinydnssec/)

```
Currently, the list of supported binary distributions for tinydnssec is

    * Arch Linux

    * SUSE
          o openSUSE_Leap_15.4
          o openSUSE_Leap_15.5
          o openSUSE_Leap_15.6
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
          o SUSE Linux Enterprise 15 SP4
          o SUSE Linux Enterprise 15 SP5
          o SUSE Linux Enterprise 15 SP6

    * Red Hat
          o Fedora 39
          o Fedora 40
          o Fedora Rawhide
          o Red Hat Enterprise Linux 7
          o Red Hat Enterprise Linux 8 +
          o Red Hat Enterprise Linux 9 +
          o EPEL 8 +
          o EPEL 9 +
          o Scientific Linux 7
          o CentOS 7
          o CentOS 8
          o CentOS 8  Stream
          o CentOS 9  Stream
          o CentOS 10 Stream
          o RockyLinux 8
          o RockyLinux 9
          o OracleLinux 8
          o OracleLinux 9
          o AlmaLinux 8
          o AlmaLinux 9
          o AmazonLinux 2023

          +: Some of the above Red Hat flavoured distributions are available
             only on copr (RHEL 8, RHEL9, EPEL9, EPEL9)

    * Debian
          o Debian 10.0
          o Debian 11.0
          o Debian 12.0

    * Ubuntu
          o Ubuntu 18.04
          o Ubuntu 20.04
          o Ubuntu 22.04
          o Ubuntu 23.04
          o Ubuntu 24.04

    * Mageia
          o Mageia 8
          o Mageia 9
```

NOTE: You can also build local binary packages. To generate RPM packages locally for all components refer to [Create Local Binary Packages](.github/CREATE-Packages.md)

## IMPORTANT NOTE for binary builds on debian

If you decide to use svscan / supervise to run dnscache, tinydns, dqcache, curvedns, you need to read this note. debian/ubuntu repositories already has daemontools which is far behind in terms of feature list that the indimail-mta repo provides. You need to ensure that the daemontools packages get installed from the indimail-mta repository instead of the debian repository. If you don't do this, tinydnssec many not function correctly as it depends on setting of proper global envirnoment variables. Global environment variables are not supported by daemontools from the official debian repository.

All you need to do is set a higher preference for the indimail-mta repository by creating /etc/apt/preferences.d/preferences with the following contents

```
$ sudo /bin/bash
# cat > /etc/apt/preferences.d/preferences <<EOF
Package: *
Pin: origin download.opensuse.org
Pin-Priority: 1001
EOF
```

You can verify this by doing

```
$ apt policy daemontools
daemontools:
  Installed: 2.11-1.1+1.1
  Candidate: 2.11-1.1+1.1
  Version table:
     1:0.76-7 500
        500 http://raspbian.raspberrypi.org/raspbian buster/main armhf Packages
 *** 2.11-1.1+1.1 1001
       1001 http://download.opensuse.org/repositories/home:/indimail/Debian_10  Packages
        100 /var/lib/dpkg/status
ucspi-tcp:
  Installed: 2.11-1.1+1.1
  Candidate: 2.11-1.1+1.1
  Version table:
     1:0.88-6 500
        500 http://raspbian.raspberrypi.org/raspbian buster/main armhf Packages
 *** 2.11-1.1+1.1 1001
       1001 http://download.opensuse.org/repositories/home:/indimail/Debian_10/ Packages
        100 /var/lib/dpkg/status
```

# SUPPORT INFORMATION

tinydnssec is supported at IndiMail

## IRC / Matrix

[![Matrix](https://img.shields.io/matrix/indimail:matrix.org.svg)](https://matrix.to/#/#indimail:matrix.org)

* [Matrix Invite Link #indimail:matrix.org](https://matrix.to/#/#indimail:matrix.org)
* IndiMail has an [IRC channel on libera](https://libera.chat/) #indimail-mta

## Mailing list

There are two Mailing Lists for IndiMail

1. indimail-support  - You can subscribe for Support [here](https://lists.sourceforge.net/lists/listinfo/indimail-support). You can mail [indimail-support](mailto:indimail-support@lists.sourceforge.net) for support Old discussions can be seen [here](https://sourceforge.net/mailarchive/forum.php?forum_name=indimail-support)
2. Archive at [Google Groups](http://groups.google.com/group/indimail). This groups acts as a remote archive for indimail-support and indimail-devel.

There is also a [Project Tracker](http://sourceforge.net/tracker/?group_id=230686) for IndiMail (Bugs, Feature Requests, Patches, Support Requests)
