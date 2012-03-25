SRCS = test.cpp
OBJS = $(SRCS:.cpp=.o)
CPP = g++
CPPFLAGS = -std=c++0x -g

all: test
test: $(OBJS)
	$(CPP) -o test $(CPPFLAGS) $(OBJS)
depends: $(SRCS)
	$(CPP) -MM $(CPPFLAGS) *.h* *.cpp > depend

.cpp.o:
	$(CPP) -c $(CPPFLAGS) $<

include depend
