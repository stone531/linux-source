# Copyright (C) 1991, 92, 93, 94, 95, 96, 97 Free Software Foundation, Inc.
# This file is part of the GNU C Library.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.

# You should have received a copy of the GNU Library General Public
# License along with the GNU C Library; see the file COPYING.LIB.  If not,
# write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.

#
#	Makefile for time routines
#
subdir	:= time

headers	:= time.h sys/time.h sys/timeb.h timebits.h
distribute := tzfile.h private.h scheck.c ialloc.c yearistype	\
	      iso3166.tab zone.tab tzselect.ksh checktab.awk	\
	      README
extra-objs = scheck.o ialloc.o $(tzfiles:%=z.%)

routines	:= offtime asctime clock ctime ctime_r difftime	\
		   gmtime localtime mktime strftime time tzset	\
		   tzfile gettimeofday settimeofday adjtime	\
		   getitimer setitimer				\
		   stime dysize timegm ftime			\
		   strptime

others	:= ap zdump zic
tests	:= test_time clocktest

tzfiles := africa antarctica asia australasia europe northamerica \
	   southamerica etcetera factory systemv backward \
	   solar87 solar88 solar89
# pacificnew doesn't compile; if it is to be used, it should be included in
# northamerica.
distribute += $(tzfiles) leapseconds pacificnew simplebackw

install-sbin := zic zdump

generated-dirs = testdata

all: # Make this the default target; it will be defined in Rules.

include ../Makeconfig	# Get objpfx defined so we can use it below.

ifeq (no,$(cross-compiling))
# We can run the test-tz test only if we can install the test data using
# zic.  This isn't possible when cross-compiling.
tests += test-tz

# Before the test-tz test can be run we need the data to be installed.
tests: install-test-data
endif

# z.* use this variable.
define nl


endef
-include $(addprefix $(objpfx)z.,$(tzfiles))

# Make these absolute file names.
installed-localtime-file := $(firstword $(filter /%,$(localtime-file)) \
					$(addprefix $(inst_zonedir)/, \
						    $(localtime-file)))
installed-posixrules-file := $(firstword $(filter /%,$(posixrules-file)) \
					 $(addprefix $(inst_zonedir)/, \
						     $(posixrules-file)))

ifeq ($(cross-compiling),no)
# Don't try to install the zoneinfo files since we can't run zic.
install-others = $(addprefix $(inst_zonedir)/,$(zonenames) \
					      $(zonenames:%=posix/%) \
					      $(zonenames:%=right/%)) \
		 $(installed-localtime-file) $(installed-posixrules-file)
endif

ifeq ($(have-ksh),yes)
install-others += $(inst_zonedir)/iso3166.tab $(inst_zonedir)/zone.tab
install-bin += tzselect
endif

include ../Rules


$(tzfiles:%=$(objpfx)z.%): $(objpfx)z.%: % Makefile
# Kludge alert: we use an implicit rule (in what we are generating here)
# because that is the only way to tell Make that the one command builds all
# the files.
	(echo 'define $*-zones'						    ;\
	 awk '$$1 == "Zone" { print $$2 } $$1 == "Link" { print $$3 }' $^   ;\
	 echo 'endef'							    ;\
	 echo '$*-zones := $$(subst $$(nl), ,$$($*-zones))'		    ;\
	 echo 'ifdef $*-zones'						    ;\
	 echo '$$(addprefix $$(inst_datadir)/zone%/right/,$$($*-zones)): \' ;\
	 echo '$< $$(objpfx)zic leapseconds yearistype'			    ;\
	 echo '	$$(tzcompile)'						    ;\
	 echo '$$(addprefix $$(inst_datadir)/zone%/posix/,$$($*-zones)): \' ;\
	 echo '$< $$(objpfx)zic /dev/null yearistype'			    ;\
	 echo '	$$(tzcompile)'						    ;\
	 echo '$$(addprefix $$(inst_datadir)/zone%/,$$($*-zones)): \'	    ;\
	 echo '$< $$(objpfx)zic $$(leapseconds) yearistype'		    ;\
	 echo '	$$(tzcompile)'						    ;\
	 echo 'endif'							    ;\
	 echo 'zonenames := $$(zonenames) $$($*-zones)'			    ;\
	) > $@.new
	mv $@.new $@

.PHONY: echo-zonenames
echo-zonenames:
	@echo 'Known zones: $(zonenames)'


# We have to use `-d $(inst_zonedir)' to explictly tell zic where to
# place the output files although $(zonedir) is compiled in.  But the
# user might have set $(install_root) on the command line of `make install'.
zic-cmd = $(built-program-cmd) -d $(inst_zonedir)
tzcompile = $(zic-cmd)$(target-zone-flavor) -L $(word 3,$^) \
	    -y $(dir $(word 4,$^))$(notdir $(word 4,$^)) $<

# The source files specify the zone names relative to the -d directory,
# so for the posix/ and right/ flavors we need to pass -d $(inst_zonedir)/posix
# and the like.  This magic extracts /posix or /right if it's the first
# component after $(inst_zonedir) in the target name $@.
target-zone-flavor = $(filter /posix /right, \
			      /$(firstword $(subst /, , \
					  $(patsubst $(inst_zonedir)/%,%,$@))))

ifdef localtime
$(installed-localtime-file): $(inst_zonedir)/$(localtime) $(objpfx)zic
	$(make-target-directory)
	if test -r $@; then \
	  echo Site timezone NOT reset to Factory.; \
	else \
	  rm -f $@T; \
	  $(SHELL) $(..)/rellns-sh $< $@T; \
	  mv -f $@T $@; \
	fi
endif
ifdef posixrules
$(installed-posixrules-file): $(inst_zonedir)/$(posixrules) $(objpfx)zic
	$(zic-cmd) -p $(posixrules)
endif


$(objpfx)zic: $(objpfx)scheck.o $(objpfx)ialloc.o

tz-cflags = -DTZDIR='"$(zonedir)"' \
	    -DTZDEFAULT='"$(localtime-file)"' \
	    -DTZDEFRULES='"$(posixrules-file)"'

CFLAGS-zdump.c = -Wno-strict-prototypes -DNOID
CFLAGS-zic.c = -Wno-strict-prototypes -DNOID $(tz-cflags)
CFLAGS-ialloc.c = -Wno-strict-prototypes -DNOID
CFLAGS-scheck.c = -Wno-strict-prototypes -DNOID
CFLAGS-tzfile.c = $(tz-cflags)
CFLAGS-tzset.c = $(tz-cflags)

# We have to make sure the data for testing the tz functions is available.
.PHONY: install-test-data
install-test-data: northamerica $(objpfx)zic $(leapseconds) yearistype \
		   etcetera simplebackw
	$(built-program-cmd) -d $(objpfx)testdata -L $(leapseconds) \
	  -y yearistype northamerica
	$(built-program-cmd) -d $(objpfx)testdata -L $(leapseconds) \
	  -y yearistype etcetera
	$(built-program-cmd) -d $(objpfx)testdata -L $(leapseconds) \
	  -y yearistype simplebackw
test-tz-ENV = TZDIR=$(objpfx)testdata


$(objpfx)tzselect: tzselect.ksh
	sed -e 's%@KSH@%$(KSH)%g' \
	    -e 's%@TZDIR@%$(zonedir)%g' < $< > $@.new
	chmod 555 $@.new
	mv -f $@.new $@

$(addprefix $(inst_zonedir)/,iso3166.tab zone.tab): $(inst_zonedir)/%: %
	$(do-install)
