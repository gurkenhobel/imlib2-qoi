include commands.mk

OPTS    := -O3
CFLAGS  := -std=c99 $(OPTS) $(shell imlib2-config --cflags) -fPIC -Wall
LDFLAGS := $(shell imlib2-config --libs) 


SRC = $(wildcard *.c)
OBJ = $(foreach obj, $(SRC:.c=.o), $(notdir $(obj)))
DEP = $(SRC:.c=.d)

LIBDIR    ?= $(shell pkg-config --variable=libdir imlib2)
LOADERDIR ?= $(LIBDIR)/imlib2/loaders/

ifndef DISABLE_DEBUG
	CFLAGS += -ggdb
endif

.PHONY: all clean

all: qoi.so

qoi.so: $(OBJ)
	$(CC) -shared -o $@ $^ $(LDFLAGS) 
	cp $@ $@.debug
	strip $@

%.o: %.c
	$(CC) -Wp,-MMD,$*.d -c $(CFLAGS) -o $@ $<

clean:
	$(RM) $(DEP)
	$(RM) $(OBJ)
	$(RM) qoi.so

install:
	$(INSTALL_DIR) $(DESTDIR)$(LOADERDIR)
	$(INSTALL_LIB) qoi.so $(DESTDIR)$(LOADERDIR)

uninstall:
	$(RM) $(PLUGINDIR)/qoi.so

-include $(DEP)