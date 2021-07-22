prefix=/usr
version=$(shell cat ../conf-version)
release=$(shell cat ../conf-release)
email=$(shell cat ../conf-email)
sysconfdir=$(shell sed -n 1p ../conf-sysconfdir)
servicedir=$(shell sed -n 1p ../conf-servicedir)
#

all: rules debian.tar.gz tinydnssec.dsc postinst preinst postrm prerm

clean:
	/bin/rm -f rules postinst postrm *.dsc debian.tar.gz _service

edit = sed \
	-e 's,@sysconfdir\@,$(sysconfdir),g' \
	-e 's,@servicedir\@,$(servicedir),g' \
	-e 's,@version\@,$(version),g' \
	-e 's,@release\@,$(release),g' \
	-e 's,@email\@,$(email),g' \
	-e 's,@prefix\@,$(prefix),g'

postrm: postrm.in ../conf-sysconfdir ../conf-servicedir
	$(edit) $@.in > $@
prerm: prerm.in ../conf-sysconfdir ../conf-servicedir
	$(edit) $@.in > $@
postinst: postinst.in ../conf-sysconfdir ../conf-servicedir
	$(edit) $@.in > $@
preinst: preinst.in ../conf-sysconfdir ../conf-servicedir
	$(edit) $@.in > $@
tinydnssec.dsc: tinydnssec.dsc.in Makefile ../conf-version ../conf-release
	$(edit) $@.in > $@
changelog: ../doc/ChangeLog Makefile ../conf-version ../conf-release \
../conf-email
	../catChangeLog --debian --name=tinydnssec --state=stable \
		--urgency=low ../doc/ChangeLog > $@
copyright: copyright.in ../conf-email
	$(edit) $@.in > $@
control: control.in ../conf-release ../conf-version ../conf-email
	$(edit) $@.in > $@
debian.tar.gz: copyright rules control changelog compat postinst \
preinst postrm prerm tinydnssec.install Makefile
	tar cf - copyright rules control changelog compat postinst \
		preinst postrm prerm tinydnssec.install |gzip > $@
