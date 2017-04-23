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
LDFLAGS   = -g -Linclude/
#CPPFLAGS += -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
#LDFLAGS +=  -stdlib=libc++

# Libraries
LDLIBS =

# Targets

all: news

install: news
	cp src/client bin
	cp src/run_server bin


# Targets rely on implicit rules for compiling and linking, the following is executed!
# $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
# $(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@
# $@ = the file name of the target of the rule
# $< = The name of the first prerequisite. If the target got its recipe from an implicit rule, this will be the first prerequisite added by the implicit rule.
# $^ = The names of all the prerequisites, with spaces between them.

# The dependency on libclientserver.a is not defined.
#myserver: myserver.o
#myclient: myclient.o

news:
	$(MAKE) -C src/
	#cp src/client client
	#cp src/run_server run_server

# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f client run_server newsgroups.db bin/client bin/run_server
	sqlite3 newsgroups.db < schema.sql
	$(MAKE) -C src/ clean

# Generate dependencies in *.d files
#%.d: %.cc
#	@set -e; rm -f $@; \
#         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
#         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#         rm -f $@.$$$$
#
## Include the *.d files
#SRC = $(wildcard include/*.cc)
##keyword include calls the "created" makefiles and runs them
#include $(SRC:.cc=.d)
