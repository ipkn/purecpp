SRCS = test.cpp
OBJS = $(SRCS:.cpp=.o)
CPP = g++
CPPFLAGS = -std=c++0x -g -ftemplate-depth-128

all: test
test: $(OBJS)
	$(CPP) -o test $(CPPFLAGS) $(OBJS)
	./test
depends: $(SRCS)
	$(CPP) -MM $(CPPFLAGS) *.h* *.cpp > depend

.cpp.o:
	$(CPP) -c $(CPPFLAGS) $<

include depend
