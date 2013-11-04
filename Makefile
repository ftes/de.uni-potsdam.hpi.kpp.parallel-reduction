CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall -std=c++11
LDFLAGS=-g
LDLIBS=-pthread

SRCS=main.cpp
OBJS=main.o
EXEC=parsum

all: $(SRCS)
	g++ $(LDFLAGS) -o $(EXEC) $(OBJS) $(LDLIBS)

%o: %cpp
	g++ -c $(CPPFLAGS) $(LDLIBS) -o $@ $<

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool
