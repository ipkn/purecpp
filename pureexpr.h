#pragma once

#include <tuple>
#include "pureargs.h"
#include "purempl.h"
#include "pureargmap.h"

namespace pure
{
	struct Expression
	{
	};

	template <typename F, typename Args>
	struct result_type
	{
		//typedef void type;
	};

	template <typename T>
	struct ConstExpression : public Expression
	{
		typedef T type;
		T t;
		ConstExpression(T t) : t(t) {}

	};
	template <typename T, int ... NArgs, typename ... CallArgs>
	ConstExpression<T> arg_bind(ConstExpression<T> e, const argmap<mpl::seq<NArgs...>, CallArgs...>&)
	{
		return e;
	}

	template <int N>
	struct VarExpression;

	namespace detail
	{
		template <int N, int ... NArgs, typename ... CallArgs>
		auto var_bind(argmap<mpl::seq<NArgs...>, CallArgs...> arg, mpl::bool_<true>)
		-> ConstExpression<typename arg_type<N, decltype(arg)>::type>
		{
			return ConstExpression<typename arg_type<N, decltype(arg)>::type>(arg_get<N>(arg));
		}

		template <int N, int ... NArgs, typename ... CallArgs>
		auto var_bind(argmap<mpl::seq<NArgs...>, CallArgs...> arg, mpl::bool_<false>)
		-> VarExpression<N>
		{
			return VarExpression<N>(Arg<N>());
		}
	}

	template <int N>
	struct VarExpression : public Expression
	{
		static const int value = N;
		VarExpression(Arg<N>){}
	};

	template <int N, int ... NArgs, typename ... CallArgs>
	auto arg_bind(VarExpression<N>, argmap<mpl::seq<NArgs...>, CallArgs...> arg)
	-> decltype(detail::var_bind<N>(arg, mpl::bool_<mpl::seq_contains<N,mpl::seq<NArgs...>>::value>()))
	{
		return detail::var_bind<N>(arg, mpl::bool_<mpl::seq_contains<N,mpl::seq<NArgs...>>::value>());
	}

#define BINARY_OPERATOR(op, className) \
	template <typename L, typename R>\
	struct className : public Expression\
	{\
		L l;\
		R r;\
		className(L l, R r):l(l),r(r){}\
		\
	};\
	template <typename L, typename R, int ... NArgs, typename ... CallArgs>		\
	auto arg_bind(className<L,R> e, argmap<mpl::seq<NArgs...>, CallArgs...> arg)		\
	-> className<decltype(arg_bind(e.l,arg)), decltype(arg_bind(e.r,arg))>\
	{		\
		return className<decltype(arg_bind(e.l,arg)), decltype(arg_bind(e.r,arg))>(arg_bind(e.l,arg), arg_bind(e.r,arg)); \
	}		\
	template <typename L, typename R>\
	className<\
	PURE_PROMOTE(L),\
	PURE_PROMOTE(R)\
	> operator op(L l, R r)\
	{\
		typedef PURE_PROMOTE(L) LE;\
		typedef PURE_PROMOTE(R) RE;\
		return className<LE, RE>(LE(l), RE(r));\
	}\
	template <typename L, typename R, typename Args>\
	struct result_type<className<L, R>, Args>\
	{\
		typedef decltype(typename result_type<L, Args>::type() op typename result_type<R, Args>::type()) type;\
	};\
	template <typename L, typename R, typename Arg>\
	auto eval(className<L, R> e, Arg arg)\
		-> decltype(eval(e.l, arg) op eval(e.r, arg))\
	{\
		return eval(e.l, arg) op eval(e.r, arg);\
	}

	//template <typename L, typename R>
	//struct AddExpression : public Expression
	//{
		//L l;
		//R r;
		//AddExpression(L l, R r):l(l),r(r){}
	//};

	template <typename F, typename ... CallArgs>
	struct CallExpression : public Expression
	{
		F f;
		std::tuple<CallArgs...> callArgs;
		CallExpression(F f, CallArgs... _callArgs):f(f), callArgs(_callArgs...){}

	};
	namespace detail 
	{
		template <typename F, typename ... CallArgs, int ... NArgs, typename ... Args, int ... S>
		auto calle_bind_helper(CallExpression<F, CallArgs...> e, const argmap<mpl::seq<NArgs...>, Args...>& arg, mpl::seq<S...>)
		-> CallExpression<
				decltype(arg_bind(e.f,arg)),	
				decltype(arg_bind(std::get<S>(e.callArgs),arg))...
			>
		{
			return CallExpression<
					decltype(arg_bind(e.f,arg)),	
					decltype(arg_bind(std::get<S>(e.callArgs),arg))...
				>
				(arg_bind(e.f,arg), arg_bind(std::get<S>(e.callArgs),arg)...);
		}
	}

	template <typename F, typename ... CallArgs, int ... NArgs, typename ... Args>
	auto arg_bind(CallExpression<F, CallArgs...> e, const argmap<mpl::seq<NArgs...>, Args...>& arg)
		-> decltype(detail::calle_bind_helper(e, arg, typename mpl::count<sizeof...(CallArgs)>::type()))
	{
		return detail::calle_bind_helper(e, arg, typename mpl::count<sizeof...(CallArgs)>::type());
	}
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
		//typedef ConstExpression<Lambda<Expr, Args...>> type;
		typedef Lambda<Expr, Args...> type;
	};


#define PURE_PROMOTE(T) typename PromoteToExpression<T>::type
	//template <typename L, typename R>
	//AddExpression<
	//PURE_PROMOTE(L),
	//PURE_PROMOTE(R)
	//> operator +(L l, R r)
	//{
		//typedef PURE_PROMOTE(L) LE;
		//typedef PURE_PROMOTE(R) RE;
		//return AddExpression<LE, RE>(LE(l), RE(r));
	//}

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
		typedef typename arg_type<N, Args>::type type;
	};

	//template <typename L, typename R, typename Args>
	//struct result_type<AddExpression<L, R>, Args>
	//{
		//typedef decltype(typename result_type<L, Args>::type()+typename result_type<R, Args>::type()) type;
	//};

	template <typename F, typename ... CallArgs, typename AMSeq, typename ... AMTypes>
	struct result_type<CallExpression<F, CallArgs...>, argmap<AMSeq, AMTypes...>>
	{
		typedef typename result_type<F, argmap<AMSeq, AMTypes...>>::type L;
		typedef typename result_type<
				typename L::expr_type,
					argmap<
						decltype(seq_cat(typename L::arg_bind_type(), AMSeq())),
						typename result_type<CallArgs, argmap<AMSeq, AMTypes...>>::type..., AMTypes...>
			>::type type;
	};

	template <typename T, typename Arg>
	T eval(ConstExpression<T> e, Arg arg)
	{
		return e.t;
	}

	//template <typename L, typename R, typename Arg>
	//auto eval(AddExpression<L, R> e, Arg arg)
		//-> decltype(eval(e.l, arg) + eval(e.r, arg))
	//{
		//return eval(e.l, arg) + eval(e.r, arg);
	//}

	template <int N, typename Arg>
	typename arg_type<N, Arg>::type eval(VarExpression<N> e, Arg arg)
	{
		return arg_get<N>(arg);
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

	template <typename OrigianlArg, typename Arg, typename F, int ... NArgs, int ... S>
	auto call_helper(Lambda<F, NArgs...> l, Arg arg, OrigianlArg oarg, mpl::seq<S...>)
		-> typename result_type<typename PromoteToExpression<F>::type, 
			decltype(make_argmap(arg, mpl::seq<NArgs...>()))
		>::type
	{
		return eval(l.e, argmap_cat(
			make_argmap(arg, mpl::seq<NArgs...>()),
			oarg	
			));
		//return l(std::get<S>(arg)...);
	}

	template <typename Arg, typename F, int ... NArgs>
	auto eval(Lambda<F, NArgs...> l, Arg arg)
		-> decltype(arg_bind(l, arg))
	{
		return arg_bind(l, arg);
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
			compute_call_args(e, arg), arg, typename mpl::count<sizeof...(CallArgs)>::type());
	}

	BINARY_OPERATOR(+, AddExpression)
	BINARY_OPERATOR(-, SubExpression)
	BINARY_OPERATOR(*, MulExpression)
	BINARY_OPERATOR(/, DivExpression)
	//BINARY_OPERATOR(<<, LShiftExpression)
	//BINARY_OPERATOR(>>, RShiftExpression)
}
