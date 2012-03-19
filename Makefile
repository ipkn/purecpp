SRCS = test.cpp
OBJS = $(SRCS:.cpp=.o)
CPP = g++-mp-4.6
CPPFLAGS = -std=c++0x -g

all: test
test: $(OBJS)
	$(CPP) -o test $(CPPFLAGS) $(OBJS)
depends:
	$(CPP) -MM *.h* *.cpp > depend

.cpp.o:
	$(CPP) -c $(CPPFLAGS) $<

include depend
