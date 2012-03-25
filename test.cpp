#include <string>
#include <iostream>
#include "pure.h"
#include "testhelper.h"
using namespace std;
using namespace pure;

TEST(Add)
{
	auto f = lambda(x, y, x+y);
	assertEqual(42, f(40, 2));
	assertEqual("abcdefghi", f(string("abc"), string("def")) + "ghi");
}

TEST(Call)
{
	auto f = lambda(x, x);
	auto g = lambda(x, x(1));
	g(f);
}
	

int main()
{
	run();
	return 0;
}
