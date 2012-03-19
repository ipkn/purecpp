#pragma once
#include <tuple>
namespace pure
{
	template <typename F, typename Args>
	struct result_type
	{
		//typedef void type;
	};

	template <typename T, typename Args>
	struct result_type<ConstExpression<T>, Args>
	{
		typedef typename ConstExpression<T>::type type;
	};

	template <int N, typename Args>
	struct result_type<VarExpression<N>, Args>
	{
		typedef typename std::tuple_element<N, Args>::type type;
	};

	template <typename L, typename R, typename Args>
	struct result_type<AddExpression<L, R>, Args>
	{
		typedef decltype(typename result_type<L, Args>::type()+typename result_type<R, Args>::type()) type;
	};

	template <typename T, typename Arg>
	T eval(ConstExpression<T> e, Arg arg)
	{
		return e.t;
	}

	template <typename L, typename R, typename Arg>
	auto eval(AddExpression<L, R> e, Arg arg)
		-> decltype(eval(e.l, arg) + eval(e.r, arg))
	{
		return eval(e.l, arg) + eval(e.r, arg);
	}

	template <int N, typename Arg>
	typename std::tuple_element<N, Arg>::type eval(VarExpression<N> e, Arg arg)
	{
		return std::get<N>(arg);
	}

	template <typename F, typename A0>
	auto apply(F f, A0 a0) -> typename result_type<typename PromoteToExpression<F>::type, std::tuple<A0>>::type
	{
		return eval(typename PromoteToExpression<F>::type(f), std::tuple<A0>(a0));
	}

	template <typename F, typename A0, typename A1>
	auto apply(F f, A0 a0, A1 a1) -> typename result_type<typename PromoteToExpression<F>::type, std::tuple<A0, A1>>::type
	{
		return eval(typename PromoteToExpression<F>::type(f), std::tuple<A0, A1>(a0, a1));
	}
}
