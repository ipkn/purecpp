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
	

int main()
{
	run();
	return 0;
}
