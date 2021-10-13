CC=gcc
LINT=splint
CTAGS=ctags
CSCOPE=cscope
RM=rm
MKDEPEND=$(CC) -MM
DOXYGEN=doxygen

INCLUDES=
CFLAGS=-c -Wall -std=c99 $(INCLUDES) -DDEBUG
LDFLAGS=

SOURCES=jmines.c game.c cli.c
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
	$(CTAGS) -R *

cscope:
	$(CSCOPE) -R -b

lint: $(SOURCES) $(HEADERS)
	$(LINT) $(SOURCES)

clean:
	-$(RM) $(SOURCES:.c=.o) $(PROGNAME) $(COREFILE)

dox: $(SOURCES) $(HEADERS)
	$(DOXYGEN)

-include .depend
