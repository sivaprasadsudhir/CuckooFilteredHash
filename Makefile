CC = g++
AR = ar
PREFIX=/usr/local

# Uncomment one of the following to switch between debug and opt mode
#OPT = -O3 -DNDEBUG
OPT = -g -ggdb

CFLAGS += --std=c++11 -fno-strict-aliasing -Wall -c -I. -I./cuckoofilter/include -I/usr/include/ 
CFLAGS += -I./cuckoofilter/src/ $(OPT) -I/usr/local/opt/openssl/include -I./libcuckoo/install/include
CFLAGS += -I./cuckoofilter/benchmarks

LDFLAGS+= -Wall -lpthread -lssl -lcrypto -L/usr/local/opt/openssl/lib

LIBOBJECTS = \
	./cuckoofilter/src/hashutil.o \

HEADERS = $(wildcard src/*.h)

SOURCES=$(wildcard src/*.cc)
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=$(SOURCES:.cc=)

all: $(EXECUTABLE)

clean:
	rm -f $(TEST) */*.o

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(@:.o=.cc) -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(@:=.o) -o $@