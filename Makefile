CC=gcc
LINT=splint
CTAGS=ctags
RM=rm
MKDEPEND=$(CC) -MM
DOXYGEN=doxygen

INCLUDES=
CFLAGS=-c -g -Wall -std=c99 $(INCLUDES) -DDEBUG -DNDEBUG -DDEBUG_LEVEL=DBG_STD
LDFLAGS=

SOURCES=jmines.c grid.c cli.c options.c debug.c errmsg.c
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(SOURCES:.c=.h) version.h
EXE=
PROGNAME=jmines$(EXE)
COREFILE=

all: $(SOURCES) $(PROGNAME)

$(PROGNAME): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

depend: $(SOURCES)
	$(MKDEPEND) $(SOURCES) > .depend

TAGS:
	$(CTAGS) *

lint: $(SOURCES) $(HEADERS)
	$(LINT) $(SOURCES)

clean:
	-$(RM) $(SOURCES:.c=.o) $(PROGNAME) $(COREFILE)

dox: $(SOURCES) $(HEADERS)
	$(DOXYGEN)

-include .depend
