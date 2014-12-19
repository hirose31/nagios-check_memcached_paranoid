# if you want to enable TRACE code:
#   make EXTRA_CFLAGS=-DDO_TRACE check_memcached_paranoid

NAGIOS_PLUGINS_DIR = /var/tmp/making/nagios-plugins-1.4.15
#NAGIOS_PLUGINS_DIR = /var/tmp/making/nagios-plugins-2.0.3
 NAGIOS_LOCALE_DIR = /usr/local/app/nagios/share/locale
   LIBMEMCACHE_DIR = /usr/local/app/libmemcached/lib

CC      = gcc
CFLAGS  = -O2 -g -Wall $(EXTRA_CFLAGS) \
	-DLOCALEDIR=\"$(NAGIOS_LOCALE_DIR)\" \
	-I$(NAGIOS_PLUGINS_DIR) -I$(NAGIOS_PLUGINS_DIR)/plugins \
	-I$(NAGIOS_PLUGINS_DIR)/gl -I$(NAGIOS_PLUGINS_DIR)/lib
LDFLAGS = \
	-rdynamic \
	-L$(NAGIOS_PLUGINS_DIR)/plugins \
	-L$(LIBMEMCACHE_DIR) -lmemcached

PROGRAM = check_memcached_paranoid

include $(NAGIOS_PLUGINS_DIR)/NP-VERSION-FILE
NP_MAJOR_VERSION = $(firstword $(subst ., ,$(NP_VERSION)))
ifeq ($(NP_MAJOR_VERSION),1)
	LDLIBS = \
		$(NAGIOS_PLUGINS_DIR)/plugins/netutils.o \
		$(NAGIOS_PLUGINS_DIR)/plugins/utils.o \
		$(NAGIOS_PLUGINS_DIR)/lib/libnagiosplug.a \
		$(NAGIOS_PLUGINS_DIR)/gl/libgnu.a \
		$(NAGIOS_PLUGINS_DIR)/plugins/popen.o
else
	LDLIBS = \
		$(NAGIOS_PLUGINS_DIR)/plugins/libnpcommon.a \
		$(NAGIOS_PLUGINS_DIR)/lib/libnagiosplug.a \
		$(NAGIOS_PLUGINS_DIR)/gl/libgnu.a
endif

all: $(PROGRAM)

clean:
	$(RM) $(PROGRAM) *~

