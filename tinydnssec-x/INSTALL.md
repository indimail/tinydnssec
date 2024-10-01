# INSTALLATION

1. create users dnscache qmaill

```
   # /usr/sbin/useradd -l -M -g nofiles  -d /etc/indimail -s /sbin/nologin dnscache
   # /usr/sbin/useradd -l -M -g nofiles  -d /etc/indimail -s /sbin/nologin qmaill
   # /usr/sbin/useradd -l -M -g nofiles  -d /etc/indimail -s /sbin/nologin tinydns
```

2. run dnscache-conf

```
   # dnscache-conf dnscache qmaill /etc/indimail/dnscache 127.0.0.1
```

3. Put service under supervise

```
   # sudo sh -c "touch /etc/indimail/dnscache/root/ip/192.168.x.x"
   # ln -s /etc/indimail/dnscache /service/dnscache
```

4. wget -O dns\_get-roots.sh https://raw.githubusercontent.com/alexh-name/admin\_duct\_tape/master/dns\_get-roots.sh
