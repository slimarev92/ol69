srcdir = ../src/
objdir = ../obj/
libdir = /home/sasha/Desktop/ds/lib/
bindir = ../bin/
hdir = ../include/
testdir = ../test/

TESTS_C = $(wildcard $(testdir)*_test.c)
SRCS = $(wildcard $(srcdir)*.c)
OBJS = $(patsubst $(srcdir)%.c, $(objdir)%.o ,$(SRCS)) 
DEPS = $(patsubst $(srcdir)%.c, %.d ,$(SRCS))
TESTDEPS = $(patsubst $(testdir)%.c, %.d ,$(TESTS_C))
TESTOBJ = $(patsubst $(testdir)%.c, $(objdir)%.o ,$(TESTS_C))
TESTBIN = $(patsubst $(testdir)%_test.c, $(bindir)%.out ,$(TESTS_C))
LIBD = $(libdir)libds_debug.so
LIBR = $(libdir)libds.so
CC=gcc
CDFLAG=-ansi -pedantic-errors -Wall -Wextra -g -lpthread
CRFLAG=-ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -lpthread

all: 
	make -s -f makefile release
	rm $(objdir)*.o
	make -s -f makefile debug
	
release:
	echo CFLAGS=$(CRFLAG) > flags.conf
	echo LIB=$(LIBR) >> flags.conf
	make -f makefile $(LIBR)

debug:
	echo CFLAGS=$(CDFLAG) > flags.conf
	echo LIB=$(LIBD) >> flags.conf
	make -f makefile $(LIBD)
	
-include flags.conf

test: debug $(TESTBIN)

testrel: release $(TESTBIN)

../bin/%.out: $(objdir)%_test.o $(libdir)libds_debug.so
	$(CC) $(CFLAGS) -o $@  $^ -lm

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

%_test.d: $(testdir)%_test.c
	$(CC) -I../include -MM -MT  $(patsubst %.d, $(objdir)%.o ,$@) $< > $@
	echo "\t"$(CC) $(CFLAGS) -c -I../include -o $(patsubst %.d, $(objdir)%.o ,$@) $< >> $@

%.d: $(srcdir)%.c
	$(CC) -I../include -MM -MT  $(patsubst %.d, $(objdir)%.o ,$@) $< > $@
	echo "\t"$(CC) $(CFLAGS) -c -I../include -fpic -o $(patsubst %.d, $(objdir)%.o ,$@) $< >> $@

-include $(DEPS)
-include $(TESTDEPS)

.PHONY: clean

clean:
	rm $(objdir)*.o || true
	rm $(bindir)*.out || true
	rm $(LIBD) $(LIBR) || true
	rm *.d
	

.PHONY: cleand
cleand: 
	rm *.d || true
	
