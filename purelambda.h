#pragma once

#include "pureargs.h"
#include "pureexpr.h"
#include "pureapply.h"

namespace pure
{
#ifndef PURE_NO_VARIADIC_TEMPLATE
	template <typename Expr, int ...Args>
	struct Lambda
	{
		typedef Expr expr_type;

		Lambda(Expr e)
		: e(e)
		{
		}

		Expr e;

		template <typename ... CallArgs>
		auto operator()(CallArgs... cargs) -> decltype(apply(e, cargs...))
		{
			return apply(e, cargs...);
		}
	};
#endif
	template <typename Expr>
	struct Lambda1
	{
		typedef Expr expr_type;

		Lambda1(Expr e)
		: e(e)
		{
		}

		Expr e;

		template <typename A0>
		auto operator()(A0 a0) -> decltype(apply(e,a0))
		{
			return apply(e, a0);
		}
	};

	template <typename Expr>
	struct Lambda2
	{
		typedef Expr expr_type;

		Lambda2(Expr e)
		: e(e)
		{
		}

		Expr e;

		template <typename A0, typename A1>
		auto operator()(A0 a0, A1 a1) -> decltype(apply(e, a0, a1))
		{
			return apply(e, a0, a1);
		}
	};

#ifdef PURE_NO_VARIADIC_TEMPLATE
	template <int N, typename Expr>
	Lambda1<Expr> lambda(Arg<N>, Expr e)
	{
		return Lambda1<Expr>(e);
	}

	template <int N, int M, typename Expr>
	Lambda2<Expr> lambda(Arg<N>, Arg<M>, Expr e)
	{
		return Lambda2<Expr>(e);
	}
#else
	template <int N, typename Expr>
	Lambda<Expr,N> lambda(Arg<N>, Expr e)
	{
		return Lambda<Expr,N>(e);
	}

	template <int N, int M, typename Expr>
	Lambda<Expr,N,M> lambda(Arg<N>, Arg<M>, Expr e)
	{
		return Lambda<Expr,N,M>(e);
	}
#endif
}
