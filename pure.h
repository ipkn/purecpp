#pragma once
#include "pureargs.h"
#include "purelambda.h"
/*
namespace pure
{
	template <int N>
	struct Arg 
	{ 
		static const int value = N; 
	};
	template <int N, typename T>
	struct TypedArg : public Arg<int> 
	{
		typedef T type;
	};

	struct Expression
	{
		template <typename Ret, typename A0, typename A1> 
		Ret apply(A0 x, A1 y)
		{
			return Ret();
		}
	};

	template <int N>
	struct VarExpression
	{
		template <typename Ret, typename A0, typename A1> 
		Ret apply(A0 x, A1 y)
		{
			if (N==0)
				return x;
			if (N==1)
				return y;
		}
	};

	template <typename L, typename R>
	struct AddExpression
	{
		apply()
		{
		}
	}

	template <typename L, typename R>
	AddExpression<L,R>  operator + (L, R)
	{
		return AddExpression<L, R>();
	}

	template <typename ExpType, typename ArgMapType>
	struct Lambda2
	{
		apply

		template<typename A0, typename A1>
		operator()(A0 a0, A1 a1)
		{
			auto arguments = std::tuple<A0, A1>(a0, a1);
			return apply(arguments);
		}
	}

	template <typename ExpType, int I0, int I1>
	Lambda2<ExpType, std::tuple<Arg<I0>, Arg<I1>>> lambda(Arg<I0>, Arg<I1>, ExpType e)
	{
		return Lambda2<ExpType, std::tuple<Arg<I0>, Arg<I1>>>(e);
	}

	Arg<0> x;
	Arg<1> y;
}

*/
