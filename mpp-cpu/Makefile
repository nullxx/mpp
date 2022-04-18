#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS	:= -Wall -Wextra -g -DLOG_USE_COLOR $(shell pkg-config --cflags gtk4)
# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = -lm $(shell pkg-config --libs gtk4)

LFLAGSTESTS = $(LFLAGS) -lcriterion

# define output directory
OUTPUT	:= output

# define source directory
SRC		:= src

# define include directory
INCLUDE	:= include
INCLUDE	:= include

# define lib directory
LIB		:= lib

# define debug flag
DEBUG ?= 0

ifeq ($(DEBUG), 1)
    CFLAGS +=-DDEBUG
else
    CFLAGS +=-DNDEBUG
endif

ifeq ($(OS),Windows_NT)
MAIN	:= main.exe
TEST	:= test.exe
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
FIXPATH = $(subst /,\,$1)
RM			:= del /q /f
MD	:= mkdir
else
MAIN	:= main
TEST	:= test
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find -L $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

# define the C source test files
TESTSOURCES := $(wildcard $(patsubst %,%/*test.c, $(SOURCEDIRS)))

# define the object files
OBJECTS		:= $(SOURCES:.c=.o)
OBJECTSTEST := $(TESTSOURCES:.c=.o)

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))
OUTPUTTEST	:= $(call FIXPATH,$(OUTPUT)/$(TEST))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS) $(OBJECTS_OBJC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(OBJECTS_OBJC) $(LFLAGS) $(LIBS)


$(TEST): $(OBJECTSTEST)
	$(CC) $(CFLAGS) -o $(OUTPUTTEST) $(OBJECTSTEST) $(LFLAGSTESTS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!%

runtest:
	./$(OUTPUTTEST)