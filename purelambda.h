#pragma once

#include "pureargs.h"
#include "pureexpr.h"
#include "pureapply.h"
#include "pureargmap.h"

namespace pure
{
	template <typename Expr, int ...Args>
	struct Lambda : public Expression
	{
		typedef typename PromoteToExpression<Expr>::type expr_type;

		typedef mpl::seq<Args...> arg_bind_type;

		Lambda(Expr e)
		: e(e)
		{
		}

		expr_type e;

		template <typename ... CallArgs>
		auto operator()(CallArgs... cargs) 
			-> decltype(eval(e, make_argmap(std::tuple<CallArgs...>(cargs...), mpl::seq<Args...>())))
		{
			static_assert(sizeof...(CallArgs) == sizeof...(Args), "Argument count is wrong. (partial application is not implemented.)");
			return eval(e, make_argmap(std::tuple<CallArgs...>(cargs...), mpl::seq<Args...>()));
		}
	};

	template <typename Expr, int ... Args, int ... NArgs, typename ... CallArgs>
	auto bind(Lambda<Expr, Args...> l, argmap<mpl::seq<NArgs...>, CallArgs...> arg)
		-> Lambda<decltype(bind(l.e,argmap_remove(arg, mpl::seq<Args...>()))), Args...>
	{
		return Lambda<decltype(bind(l.e,argmap_remove(arg, mpl::seq<Args...>()))), Args...>(bind(l.e,argmap_remove(arg, mpl::seq<Args...>())));
	}

	template <typename Expr>
	Lambda<Expr> lambda(Expr e)
	{
		return Lambda<Expr>(e);
	}

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

	template <int N1, int N2, int N3, typename Expr>
	Lambda<Expr,N1,N2,N3> lambda(Arg<N1>, Arg<N2>, Arg<N3>, Expr e)
	{
		return Lambda<Expr,N1,N2,N3>(e);
	}
}
