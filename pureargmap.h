#pragma once
#include <tuple>
#include "purempl.h"

namespace pure
{
	template <typename Seq, typename ... Types>
	struct argmap
	{
		std::tuple<Types...> data;
		Seq seq;
		argmap(std::tuple<Types...> data, Seq seq) : data(data), seq(seq){}
	};


	template <typename Seq, typename ... Types>
	argmap<Seq, Types...>
	make_argmap(std::tuple<Types...> data, Seq seq)
	{
		return argmap<Seq, Types...>(data, seq);
	}

	template <int N, typename T>
	struct arg_type;
	template <int N, typename Seq, typename ... Types>
	struct arg_type<N, argmap<Seq, Types...>>
	{
		typedef typename std::tuple_element<mpl::find_seq<N,Seq>::value, std::tuple<Types...>>::type type;
	};

	template <int N, typename Seq, typename ... Types>
	auto arg_get(argmap<Seq, Types...> t)
	-> typename arg_type<N, argmap<Seq, Types...>>::type
	{
		return std::get< mpl::find_seq<N, Seq>::value >(t.data);
	}

	template <typename Seq1, typename Seq2, typename ... Types1, typename ... Types2>
	argmap<
		decltype(mpl::seq_cat(Seq1(),Seq2())),
		Types1..., Types2...
		> argmap_cat(argmap<Seq1, Types1...> am1, argmap<Seq2, Types2...> am2)
		{
			return argmap<
				decltype(mpl::seq_cat(Seq1(),Seq2())),
				Types1..., Types2...
				> (
					tuple_cat(am1.data, am2.data),
					mpl::seq_cat(am1.seq, am2.seq)
				);
		}

	template <typename Seq1, int ... S, typename ... Types1>
	auto argmap_select_helper(argmap<Seq1, Types1...> am, mpl::seq<S...>)
		->decltype(make_argmap(
			std::tuple<typename arg_type<S, decltype(am)>::type...>(
				arg_get<S>(am)...
				),
			mpl::seq<S...>()
			))
	{
		return make_argmap(
			std::tuple<typename arg_type<S, decltype(am)>::type...>(
				arg_get<S>(am)...
				),
			mpl::seq<S...>()
			);
	}

	template <typename Seq1, typename Seq2, typename ... Types1>
	auto argmap_select(argmap<Seq1, Types1...> am, Seq2)
		-> decltype(argmap_select_helper(am, typename mpl::seq_and<Seq1, Seq2>::type()))
	{
		return argmap_select_helper(am, typename mpl::seq_and<Seq1, Seq2>::type());
	}

	template <typename Seq1, typename Seq2, typename ... Types1>
	auto argmap_remove(argmap<Seq1, Types1...> am, Seq2 s)
		-> decltype(argmap_select(am, typename mpl::seq_diff<Seq1, Seq2>::type()))
	{
		return argmap_select(am, typename mpl::seq_diff<Seq1, Seq2>::type());
	}
}
