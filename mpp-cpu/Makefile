#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# this is prepared for a unix like system

# define the C compiler to use
CC = emcc
# TODO EXPORTED_METHODS
#EXPORTED_METHODS := linker_init linker_shutdown linker_get_rom_pos linker_get_next_state linker_cancel_run linker_is_running linker_run linker_get_watchers linker_get_current_state_updated linker_get_data_bus_updated linker_get_dir_bus_updated linker_get_control_bus_updated linker_get_mem_updated
# -s EXPORTED_RUNTIME_METHODS=['ccall']
# define any compile-time flags
CFLAGS	:= -Wall -Wextra -g -DLOG_USE_COLOR
# FIXME EXPORT_ALL=1 for debugging
EMCCFLAGS := -s EXPORT_ALL=1 --no-entry

LFLAGS = -lm

LFLAGSTESTS = $(LFLAGS)

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

MAIN	:= libmpp.wasm
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find -L $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

# define the object files
OBJECTS		:= $(SOURCES:.c=.o)

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(EMCCFLAGS) -o $(OUTPUT)/$@ $^ $(LFLAGS) $(LIBS)

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