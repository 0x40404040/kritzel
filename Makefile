CC 			= gcc
CFLAGS 		= -Wall -Wextra -std=c11 -Wno-unused-parameter
INCLUDES 	:= `pkg-config --cflags gtk+-3.0 goocanvas-2.0`
LDLIBS 		:= `pkg-config --libs gtk+-3.0 goocanvas-2.0`


BUILDDIR 	= build
SOURCEDIR 	= src
BINNAME 	= kritzel
BINARY 		= $(BUILDDIR)/$(BINNAME)

SOURCES		:= $(wildcard $(SOURCEDIR)/*.c) 
OBJECTS 	:= $(SOURCES:$(SOURCEDIR)/%.c=$(BUILDDIR)/%.o)

.PHONY: all

all: build

run: build
	./$(BINARY)

build: $(OBJECTS) 
	$(CC) $^ -o $(BINARY) $(LDLIBS)

$(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 
