all: test
test: test.cpp
	g++ -o test -std=c++0x test.cpp
depends:
	g++ -MM *.h* *.cpp > depend

include depend
