#pragma once

#include <tuple>
#include "pureexpr.h"

namespace pure
{

#ifdef PURE_NO_VARIADIC_TEMPLATE
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
#else
	template <typename F, typename... Args>
	auto apply(F f, Args... args) -> typename result_type<typename PromoteToExpression<F>::type, std::tuple<Args...>>::type
	{
		return eval(typename PromoteToExpression<F>::type(f), std::tuple<Args...>(args...));
	}

#endif
}
