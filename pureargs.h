#pragma once
namespace pure
{
	template <int N>
	struct Arg
	{
		static const int value = N;
	};

	Arg<0> x;
	Arg<1> y;
}
