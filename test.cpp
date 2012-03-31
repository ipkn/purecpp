#include <string>
#include <typeinfo>
#include <iostream>
#include "testhelper.h"
#include "pure.h"
using namespace std;
using namespace pure;

TEST(LambdaArgumentChange)
{
	auto f = lambda(y, y);
	assertEqual(1, f(1));
	assertEqual(2, f(2));
	assertEqual(string("abc"), f("abc"));
	auto g = lambda(lambda(x,x));
	auto h = g();
	assertEqual(1, h(1));
}

TEST(LispLikeList)
{
	auto cons = lambda(x, y, lambda(z, z(x,y)));
	auto car = lambda(x, x(lambda(x, y, x)));
	auto cdr = lambda(x, x(lambda(x, y, y)));
	auto nil = 0;
	auto list = cons(1, cons(2, cons(3, nil)));
	//assertEqual(1, car(list));
	//assertEqual(2, car(cdr(list)));
	//assertEqual(3, car(cdr(cdr(list))));
}

TEST(BinaryOp)
{
	auto f = lambda(x, y, x+y);
	assertEqual(42, f(40, 2));
	assertEqual("abcdefghi", f(string("abc"), string("def")) + "ghi");

	auto g = lambda(x, y, (x-y)*7);
	assertEqual(49, g(11,4));
	auto h = lambda(x, y, (x-3)/y);
	assertEqual(2, h(11,4));
}

TEST(SimpleCall)
{
	auto f2= lambda(1);
	assertEqual(1, f2());

	auto f = lambda(x, x);
	auto f1 = lambda(x, x+1);
	auto g = lambda(x, x(1));
	assertEqual(1,g(f));
	assertEqual(2,g(f1));
	assertEqual(1,f(g(f)));
	assertEqual(1,f(g)(f));

	auto h = lambda(x, y, x(y));
	assertEqual(4,h(f, 4));
	assertEqual(5,h(f1, 4));

	assertEqual(2,h(g, f1));

	auto k = lambda(x, y, x(y+4)+5);
	assertEqual(10,k(f, 1));
	assertEqual(11,k(f, 2));
}
	

int main()
{
	run();
	return 0;
}
