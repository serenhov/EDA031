# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstd++.
CPPFLAGS  = -std=c++11 -Iinclude/
CXXFLAGS  = -g -O2 -Wall -W -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast
CXXFLAGS += -std=c++11
LDFLAGS   = -g
#CPPFLAGS += -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
#LDFLAGS +=  -stdlib=libc++


# Libraries
#LDLIBS = -L /usr/lib/x86_64-linux-gnu -lsqlite3
LDLIBS = -lsqlite3

VPATH = include:deps

# Targets
PROGS = client inmemory #atdisk

all: $(PROGS)

# Targets rely on implicit rules for compiling and linking, the following is executed!
# $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
# $(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@
# $@ = the file name of the target of the rule
# $< = The name of the first prerequisite. If the target got its recipe from an implicit rule, this will be the first prerequisite added by the implicit rule.
# $^ = The names of all the prerequisites, with spaces between them.

# The dependency on libclientserver.a is not defined.

client: client.o messagehandler.o connection.o

inmemory: CPPFLAGS+=-DINMEMORY=1
inmemory: run_server.o inmemory.o atdisk.o server.o connection.o messagehandler.o

atdisk: CPPFLAGS+=-DINMEMORY=0
atdisk: run_ad_server.o atdisk.o inmemory.o server.o connection.o messagehandler.o

run_im_server.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ server.cc
	#$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

run_ad_server.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ server.cc

# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f *.o client inmemory atdisk deps/*.d

# Generate dependencies in *.d files
# The line starting with an @ indicates print-suppression. Lines ending with a backslash indicate line splitting.
deps/%.d: %.cc | deps
	@set -e; rm -f $@; \
	 $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

deps:
	mkdir -p deps;

# Include the *.d files
#SRC = $(wildcard *.cc)
#keyword include calls the "created" makefiles and runs them
#include deps/$(SRC:.cc=.d)
SRC = $(wildcard *.cc)
SRC := $(addprefix deps/, $(SRC))
include $(SRC:.cc=.d)

#Target which allows you to print variables as "make print-VARIABLE"
print-%  : ; @echo $* = $($*)

