## SOURCE Installation Steps

### Installation of the tinydnssec patch

### Requirements

This patch is *not* against stock djbdns. Here's the minimal set of patches
to install before the tinydnssec patch applies:

```
1. http://www.fefe.de/dns/djbdns-1.05-test27.diff.bz2
   Unfortunately, fefe refuses to name a license for this patch, which means
   that I cannot redistribute it.

2. My own fixes to the Makefile (IPv6-related): djbdns-ipv6-make.patch
```

### Build

1. Download and unpack the original djbdns sources from
   http://cr.yp.to/djbdns/install.html .
2. Download and apply the patches listed above.
3. Download and unpack http://tinydnssec.org/tinydnssec-1.05-1.tar.gz in
   the top-level source directory.
4. Apply djbdns-1.05-dnssec.patch.
5. Install as per usual instructions (see http://cr.yp.to/djbdns/install.html ).
6. Optional: run tests (see below).
7. Install djbdns as per original instructions, or whatever your preferred
   method is.
8. Install tinydns-sign.pl in your preferred location for system 
   executables, like e. g. /usr/sbin .
9. Optional: create a manpage for tinydns-sign using e. g.

   `pod2man -s 8 -c '' "tinydns-sign.pl" >tinydns-sign.8`

   then install it in your preferred location for system manpages.
   NOTE: On arch linux pod2man will be in /usr/bin/core\_perl


### Test

run-tests.sh will sign test/data using keys from test/example\*, then issue some queries using tinydns-get, i.e. without any networking involved.

As root, start tinydns / axfrdns on a local address (127.0.0.3), then execute

`SERVER=127.0.0.3 run-tests.sh -t -u`

to test the same queries via tcp and udp.

## dnscache

The internet has evolved a lot since djbdns was written, and dnscache does not handle todays (questionable) site setups spread over multiple domains very well. In most cases using a cache like unbound will perform significantly better.

If you still want to run dnscache follow the instructions on the djbdns hompage: http://cr.yp.to/djbdns/run-cache.html

## tinydns

For a basic setup, do the inital configuration with tinydns-conf:

```
# tinydns-conf tinydns dnslog /etc/indimail/tinydns <your-ip>
```

Then put your configuration in /etc/tinydns/root/data, build the data.cdb file using "make", and start the server via systemd:

`# svc -u /service/tinydns`

Check with "svstat /service/tinydns" if the server is running. You should now be able to query the configured DNS records. Check the djbdns page for more details on adding records: http://cr.yp.to/djbdns/run-server.html
