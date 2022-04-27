# this is prepared for a unix like system

# define standard colors
ifneq (,$(findstring xterm,${TERM}))
	BLACK        := $(shell tput -Txterm setaf 0)
	RED          := $(shell tput -Txterm setaf 1)
	GREEN        := $(shell tput -Txterm setaf 2)
	YELLOW       := $(shell tput -Txterm setaf 3)
	LIGHTPURPLE  := $(shell tput -Txterm setaf 4)
	PURPLE       := $(shell tput -Txterm setaf 5)
	BLUE         := $(shell tput -Txterm setaf 6)
	WHITE        := $(shell tput -Txterm setaf 7)
	RESET := $(shell tput -Txterm sgr0)
else
	BLACK        := ""
	RED          := ""
	GREEN        := ""
	YELLOW       := ""
	LIGHTPURPLE  := ""
	PURPLE       := ""
	BLUE         := ""
	WHITE        := ""
	RESET        := ""
endif

ifeq ($(RUNTEST), 1)
    CC=gcc
else
    CC=emcc
endif

CFLAGS	:= -Wall -Wextra -g
EMCCFLAGS := --no-entry -s MODULARIZE=1 -s ENVIRONMENT='web' -s EXPORT_ES6=1 -s 'EXPORTED_RUNTIME_METHODS=["cwrap", "getValue", "addFunction"]' -s ALLOW_TABLE_GROWTH=1 -s SAFE_HEAP=1 -O3

LFLAGS = -lm

LFLAGSTESTS = $(LFLAGS)

OUTPUT	:= output

SRC		:= src

DEBUG ?= 0

ifeq ($(DEBUG), 1)
    CFLAGS +=-DDEBUG
endif

MAIN	:= libmpp.js

SOURCEDIRS	:= $(shell find $(SRC) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p

SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))
OBJECTS		:= $(SOURCES:.c=.o)

TEST := test
TESTDIRS := $(shell find $(TEST) -type d -not -path '*test/_munit*')
TESTSOURCES := $(wildcard $(patsubst %,%/*.c, $(TESTDIRS)))
TESTOBJECTS := $(shell find $(TEST) -type f  \( -iname "*.o" \) -not -path '*test/_munit*')

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

compile-tests: $(OBJECTS)
	@for path in $(TESTSOURCES) ; do \
        gcc test/_munit/munit.c $$path $(OBJECTS) $(LFLAGS) -o $$path.o ; \
    done

run-tests:
	@for path in $(TESTOBJECTS) ; do \
        ./$$path && echo "${GREEN}PASS${RESET}: $$path"  || (echo "${RED}FAILED${RESET}: $$path" && exit 1) ; \
    done
$(OUTPUT):
	$(MD) $(OUTPUT)
# echo "Test $$path ended with status ${GREEN}$$?${RESET}"
$(MAIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(EMCCFLAGS) -fPIC -o $(OUTPUT)/$@ $^ $(LFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(shell find -type f -name "*.o")
	@echo Cleanup complete!