CC 			= gcc
CFLAGS 		= -Wall -Wextra -std=c11 -Wno-unused-parameter
INCLUDES 	:= 
LDLIBS 		:=


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
	$(CC) $^ -o $(BINARY)

$(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: 
	@echo $(SOURCES)
	@echo $(OBJECTS)
	@echo $(SOURCES:$(SOURCEDIR)/%.c=$(BUILDDIR)/%.o)

