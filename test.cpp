#include <string>
#include <typeinfo>
#include <iostream>
#include "testhelper.h"
#include "pure.h"
using namespace std;
using namespace pure;

TEST(LispLikeList)
{
	auto cons = lambda(x, y, lambda(z, z(x,y)));
	auto car = lambda(x, x(lambda(x, y, x)));
	auto cdr = lambda(x, x(lambda(x, y, y)));
	auto nil = 0;

	{
		auto list = cons(1, 2);
		auto h = lambda(x,y,x);
		assertEqual(1,list(h));
		assertEqual(2,list(lambda(x,y,y)));
		assertEqual(1, car(list));
		assertEqual(2, cdr(list));
	}
	{
		auto list = cons(1, cons(2, cons(3, nil)));
		assertEqual(1, car(list));
		assertEqual(2, car(cdr(list)));
		assertEqual(3, car(cdr(cdr(list))));
	}
}

TEST(Internal)
{
	typedef mpl::seq<1,2,3> seq_type;
	assert(mpl::seq_contains<1, seq_type>::value, "Contains");
	assert(!mpl::seq_contains<4, seq_type>::value, "Not contains");
	VarExpression<2> v1((Arg<2>()));
	VarExpression<4> v2((Arg<4>()));
	argmap<seq_type, int, int, int> arg(std::tuple<int,int,int>(10,11,12),seq_type());
	auto x1 = arg_bind(v1,arg);
	auto x2 = arg_bind(v2,arg);
	assertEqual(11, x1.t);
	assertEqual(typeid(x2).name(), typeid(v2).name());
	Arg<10> a;
	auto addx = a+y;
	auto binded = arg_bind(addx,arg);
	assertEqual(21, eval(binded, make_argmap(std::tuple<int>(11), mpl::seq<10>())));
	auto l = lambda(a, a+y);

	argmap_select(arg, mpl::seq<10>());
	mpl::seq_diff<seq_type,mpl::seq<10>>::type t;
	argmap_remove(arg, mpl::seq<10>());
	// l(11) - compiler error
	auto l2 = arg_bind(l, arg);
	assertEqual(21, l2(11));
}

TEST(Closure)
{
	{
		auto f = lambda(x, lambda(x, x));
		auto g = f(1);
		assertEqual(2, g(2));
	}

	{
		auto f = lambda(x, lambda(y, x+y));
		auto g = f(5);
		assertEqual(10, g(5));
		auto h = f(string("abc"));
		assertEqual("abcdef", h("def"));
	}
}

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
