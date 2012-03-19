#pragma once

#include <tuple>
#include "pureargs.h"

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


	template <typename T> 
	struct PromoteToExpression
	{
		//BOOST_STATIC_ASSERT(std::is_base_of<Expression, T>::value);
		static_assert(std::is_base_of<Expression, T>::value, "T is not Expression");
		typedef T type;
	};

	template <int N>
	struct PromoteToExpression<Arg<N>>
	{
		typedef VarExpression<N> type;
	};

	template <>
	struct PromoteToExpression<int>
	{
		typedef ConstExpression<int> type;
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
}
