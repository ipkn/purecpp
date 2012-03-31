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
}
