# this is prepared for a unix like system

CC = emcc
CFLAGS	:= -Wall -Wextra -g -DLOG_USE_COLOR
EMCCFLAGS := --no-entry -s MODULARIZE=1 -s ENVIRONMENT='web' -s EXPORT_ES6=1 -s 'EXPORTED_RUNTIME_METHODS=["cwrap", "getValue", "addFunction"]' -s ALLOW_TABLE_GROWTH=1 -s SAFE_HEAP=1

LFLAGS = -lm

LFLAGSTESTS = $(LFLAGS)

OUTPUT	:= output

SRC		:= src

INCLUDE	:= include

LIB		:= lib

DEBUG ?= 0

ifeq ($(DEBUG), 1)
    CFLAGS +=-DDEBUG
else
    CFLAGS +=-DNDEBUG
endif

MAIN	:= libmpp.js
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find -L $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p

INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

OBJECTS		:= $(SOURCES:.c=.o)

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(EMCCFLAGS) -fPIC -o $(OUTPUT)/$@ $^ $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	@echo Cleanup complete!