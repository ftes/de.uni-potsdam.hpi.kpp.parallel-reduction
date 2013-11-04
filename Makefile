CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall -std=c++11
LDFLAGS=-g
LDLIBS=-pthread

SRCS=main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
EXEC=parsum

all: $(OBJS)
	g++ $(LDFLAGS) -o $(EXEC) $(OBJS) $(LDLIBS)

%.o: %.cpp
	g++ -c $(CPPFLAGS) $(LDLIBS) -o $@ $<

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool
