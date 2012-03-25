#pragma once

#include <tuple>
#include "pureargs.h"
#include "purempl.h"

namespace pure
{
	struct Expression
	{
	};

	template <int N>
	struct VarExpression : public Expression
	{
		static const int value = N;
		VarExpression(Arg<N>){}
	};

	template <typename T>
	struct ConstExpression : public Expression
	{
		typedef T type;
		T t;
		ConstExpression(T t) : t(t) {}
	};

	template <typename L, typename R>
	struct AddExpression : public Expression
	{
		L l;
		R r;
		AddExpression(L l, R r):l(l),r(r){}
	};

	template <typename F, typename ... CallArgs>
	struct CallExpression : public Expression
	{
		F f;
		std::tuple<CallArgs...> callArgs;
		CallExpression(F f, CallArgs... _callArgs):f(f), callArgs(_callArgs...){}
	};

	template <typename T> 
	struct PromoteToExpression
	{
		typedef typename mpl::if_<std::is_base_of<Expression, T>, T, ConstExpression<T>>::type type;
	};

	template <int N>
	struct PromoteToExpression<Arg<N>>
	{
		typedef VarExpression<N> type;
	};

	template <typename Expr, int ...Args>
	struct Lambda;

	template <typename Expr, int ...Args>
	struct PromoteToExpression<Lambda<Expr, Args...>>
	{
		typedef Expr type;
	};


#define PURE_PROMOTE(T) typename PromoteToExpression<T>::type
	template <typename L, typename R>
	AddExpression<
	PURE_PROMOTE(L),
	PURE_PROMOTE(R)
	> operator +(L l, R r)
	{
		typedef PURE_PROMOTE(L) LE;
		typedef PURE_PROMOTE(R) RE;
		return AddExpression<LE, RE>(LE(l), RE(r));
	}

	template <typename F, typename Args>
	struct result_type
	{
		//typedef void type;
	};

	template <typename Expr, typename Args, int... NArgs>
	struct result_type<Lambda<Expr, NArgs...>, Args>
	{
		typedef Lambda<Expr, NArgs...> type;
	};

	template <typename T, typename Args>
	struct result_type<ConstExpression<T>, Args>
	{
		typedef T type;
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

	template <typename Args, typename F, typename ... CallArgs>
	struct result_type<CallExpression<F, CallArgs...>, Args>
	{
		typedef typename result_type<
				typename result_type<F, Args>::type::expr_type,
				std::tuple<typename result_type<CallArgs, Args>::type...>
			>::type type;
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

	template <typename F, typename Arg, typename ... CallArgs, int ... S>
	std::tuple<typename result_type<CallArgs, Arg>::type...>
	compute_call_args_helper(CallExpression<F, CallArgs...>ce, Arg arg, mpl::seq<S...>)
	{
		return std::tuple<typename result_type<CallArgs, Arg>::type...>(
				eval(std::get<S>(ce.callArgs),arg)...
			);
	}
	template <typename F, typename Arg, typename ... CallArgs>
	std::tuple<typename result_type<CallArgs, Arg>::type...>
	compute_call_args(CallExpression<F, CallArgs...>ce, Arg arg)
	{
		return compute_call_args_helper(ce, arg, typename mpl::count<sizeof...(CallArgs)>::type());
		
	}

	template <typename Arg, typename F, int ... NArgs, int ... S>
	auto call_helper(Lambda<F, NArgs...> l, Arg arg, mpl::seq<S...>)
		-> typename result_type<typename PromoteToExpression<F>::type, Arg>::type
	{
		return l(std::get<S>(arg)...);
	}

	template <typename Arg, typename F, int ... NArgs>
	auto eval(Lambda<F, NArgs...> l, Arg arg)
		-> typename result_type<typename PromoteToExpression<F>::type, Arg>::type
	{
		return l;
	}

	template <typename F, typename Arg, typename ... CallArgs>
	auto eval(CallExpression<F, CallArgs...> e, Arg arg)
		-> typename result_type<CallExpression<F, CallArgs...>, Arg>::type
	{
		return 
			call_helper(
				eval(
					typename PromoteToExpression<F>::type(e.f), 
					arg
				),
			compute_call_args(e, arg), typename mpl::count<sizeof...(CallArgs)>::type());
	}
}
