NAGIOS_PLUGINS_DIR=/var/tmp/making/nagios-plugins-1.4.13

CC      = gcc
CFLAGS  = -O2 -g -Wall -DLOCALEDIR=\"/usr/local/app/nagios/share/locale\" -I$(NAGIOS_PLUGINS_DIR) -I$(NAGIOS_PLUGINS_DIR)/plugins -I$(NAGIOS_PLUGINS_DIR)/gl -I$(NAGIOS_PLUGINS_DIR)/lib
LDFLAGS = $(NAGIOS_PLUGINS_DIR)/plugins/netutils.o $(NAGIOS_PLUGINS_DIR)/plugins/utils.o $(NAGIOS_PLUGINS_DIR)/lib/utils_base.o -rdynamic -L$(NAGIOS_PLUGINS_DIR)/plugins -L/usr/local/app/libmemcache/lib -lmemcache

all:
	@echo blah blah blha

check_memcached_paranoid: check_memcached_paranoid.c

clean:
	$(RM) check_memcached_paranoid *~
