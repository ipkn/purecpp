
#include <typeinfo>
#include <iostream>
#include "pure.h"
using namespace std;
using namespace pure;

template <typename F>
void g(F f )
{
	cout << f(60, 5) << endl;
}

int main()
{
	auto f = lambda(x, y, x+y);
	cout << f(40, 2) << endl;
	g(f);
	return 0;
}
