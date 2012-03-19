#pragma once
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
}
