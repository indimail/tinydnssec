prefix=/usr
version=$(shell cat ../conf-version)
release=$(shell cat ../conf-release)
email=$(shell cat ../conf-email)
sysconfdir=/etc/indimail
#

all: rules debian.tar.gz dummy.tar.gz tinydnssec.dsc postinst preinst postrm prerm

clean:
	/bin/rm -f rules postinst postrm *.dsc debian.tar.gz dummy.tar.gz _service

edit = sed \
	-e 's,@sysconfdir\@,$(sysconfdir),g' \
	-e 's,@version\@,$(version),g' \
	-e 's,@release\@,$(release),g' \
	-e 's,@email\@,$(email),g' \
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
changelog: ../doc/ChangeLog Makefile ../conf-version ../conf-release \
../conf-email
	../catChangeLog --debian --name=tinydnssec --state=unstable \
		--urgency=low ../doc/ChangeLog > $@
copyright: copyright.in ../conf-email
	$(edit) $@.in > $@
control: control.in ../conf-release ../conf-version ../conf-email
	$(edit) $@.in > $@
dummy.tar.gz: README
	tar cf - README |gzip > $@
debian.tar.gz: copyright rules control changelog compat postinst \
preinst postrm prerm tinydnssec.install Makefile
	tar cf - copyright rules control changelog compat postinst \
		preinst postrm prerm tinydnssec.install |gzip > $@
