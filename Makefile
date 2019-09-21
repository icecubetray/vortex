#
#	Configurable variables
#

CC = gcc
LD = gcc

PROFILE = drebug

CFLAGS = -fPIC
CFLAGS_DEBUG = -g -DDEBUG=1
CFLAGS_RELEASE = -O3 -DRELEASE=1

LFLAGS = -fPIC --shared

OBJS = $(patsubst %.c, %.o, $(shell find src/ -type f -name '*.c'))
OUT_DIR = bin




#
#	Computed variables
#

PROFILE_SANE = $(shell echo "$(PROFILE)" | sed 's/[[:space:]\-]/_/g' | tr [:lower:] [:upper:] | sed 's/[^A-Za-z0-9_]//g')

CFLAGS_PROFILE = $(CFLAGS_$(PROFILE_SANE))
LFLAGS_PROFILE = $(LFLAGS_$(PROFILE_SANE))




.PHONY: all libvortex mostlyclean clean

all: libvortex

libvortex: $(OUT_DIR)/libvortex.so




mostlyclean:
	rm -f $(OBJS);


clean: mostlyclean
	rm -rf $(OUT_DIR)




# Wildcard target for object files
%.o: %.c
	$(CC) -o $@ $(CFLAGS) $(CFLAGS_PROFILE) -c $^




# Vortex library target
$(OUT_DIR)/libvortex.so: $(OBJS)
	@mkdir -p $(dir $@)
	$(LD) -o $@ $(LFLAGS) $^
ifeq (release,$(PROFILE))
	strip --strip-unneeded $@
endif
