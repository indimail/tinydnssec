prefix=/usr
version=$(shell cat ../conf-version)
release=$(shell cat ../conf-release)
sysconfdir=/etc/indimail
#

all: rules debian.tar.gz dummy.tar.gz tinydnssec.dsc postinst preinst postrm prerm

clean:
	/bin/rm -f rules postinst postrm *.dsc debian.tar.gz dummy.tar.gz _service

edit = sed \
	-e 's,@sysconfdir\@,$(sysconfdir),g' \
	-e 's,@version\@,$(version),g' \
	-e 's,@release\@,$(release),g' \
	-e 's,@prefix\@,$(prefix),g'

postrm: postrm.in
	$(edit) $@.in > $@
prerm: prerm.in
	$(edit) $@.in > $@
postinst: postinst.in
	$(edit) $@.in > $@
preinst: preinst.in
	$(edit) $@.in > $@
tinydnssec.dsc: tinydnssec.dsc.in Makefile
	$(edit) $@.in > $@
changelog: changelog.in ../conf-release ../conf-version
	(cat $@.in; echo "";\
	echo " -- Manvendra Bhangui <manvendra@indimail.org>  `date +'%a, %d %b %Y %H:%S:%M %z'`" )| $(edit) > $@
dummy.tar.gz: README
	tar cf - README |gzip > $@
debian.tar.gz: copyright rules control changelog compat postinst \
preinst postrm prerm tinydnssec.install Makefile
	tar cf - copyright rules control changelog compat postinst \
		preinst postrm prerm tinydnssec.install |gzip > $@
