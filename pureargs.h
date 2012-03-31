#pragma once
namespace pure
{
	template <typename F, typename ... Args>
	struct CallExpression;
	template <int N>
	struct VarExpression;
	template <typename T> 
	struct PromoteToExpression;

	template <int N>
	struct Arg
	{
		static const int value = N;

		template <typename ... ArgExprs>
		auto operator ()(ArgExprs... args)
			-> CallExpression<VarExpression<N>, typename PromoteToExpression<ArgExprs>::type... >
		{
			return CallExpression<VarExpression<N>, typename PromoteToExpression<ArgExprs>::type...>(VarExpression<N>(Arg<N>()), typename PromoteToExpression<ArgExprs>::type(args)...);
		}
	};

	Arg<0> x;
	Arg<1> y;
	Arg<2> z;
}
