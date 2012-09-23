CC=gcc
LINT=splint
CTAGS=ctags
RM=rm
MKDEPEND=$(CC) -MM

INCLUDES=
CFLAGS=-c -Wall -ansi $(INCLUDES) -DDEBUG
LDFLAGS=-l ncurses

SOURCES=jmines.c game.c cli.c
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(SOURCES:.c=.h)
EXE=
PROGNAME=jmines$(EXE)
COREFILE=

all: $(SOURCES) $(PROGNAME)

$(PROGNAME): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

depend: $(SOURCES)
	$(MKDEPEND) > .depend

TAGS:
	$(CTAGS) *

lint: $(SOURCES) $(HEADERS)
	$(LINT) $(SOURCES)

clean:
	-$(RM) $(SOURCES:.c=.o) $(PROGNAME) $(COREFILE)

-include .depend