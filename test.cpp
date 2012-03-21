
#include <string>
#include <typeinfo>
#include <iostream>
#include "pure.h"
using namespace std;
using namespace pure;

struct TestCase
{
	void assert();
};

#define TEST(x) \
	struct S__##x : public TestCase \
	{ \
		S__##x(); \
	} inst_S__##x; \
	S__##x::S__##x()

TEST(first)
{
	cout << "TEST" << endl;
}
	

template <typename F>
void g(F f )
{
	cout << f(60, 5) << endl;
}

int main()
{
	auto f = lambda(x, y, x+y);
	cout << f(40, 2) << endl;
	cout << f(string("abc"), string("def")) + "ghi"<< endl;
	g(f);
	return 0;
}
