#pragma once
#include "pure.h"

#ifdef PURE_USE_BOOST
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/map.hpp>
namespace pure
{
	namespace mpl
	{
		using namespace boost::mpl;
	}
}
#else
#error boost required currently.
#endif
namespace pure
{
	namespace mpl
	{
		// counter
		template <int...> struct seq{};

		template <int N, int ...S>
			struct count : count<N-1, N-1, S...> {};

		template <int ... S>
			struct count<0, S...>
			{
				typedef seq<S...> type;
			};

		template <int ... A, int ... B>
		seq<A..., B...> seq_cat(seq<A...>, seq<B...>)
		{
			return seq<A..., B...>();
		}

		// 
		template <int ...> struct find;

		template <int N, int P> 
		struct find<N, P>
		{
			//static_assert(false, "not exists");
		};

		template <int N, int P, int F, int ... S>
			struct find<N, P, F, S...>
			{
				static const int value = 
					if_c<N==F,
						int_<P>,
						find<N, P+1, S...>
					>::type::value;
			};

		template <int N, typename T>
		struct find_seq;

		template <int N, int ... S>
		struct find_seq<N, seq<S...>>
		{
			static const int value = find<N, 0, S...>::value;
		};

		template <int N, typename T>
		struct seq_contains;

		template <int N>
		struct seq_contains<N, seq<>>
		{
			static const bool value = false;
		};

		template <int N, int F, int ... S>
		struct seq_contains<N, seq<F, S...>>
		{
			static const bool value = if_c<N==F, bool_<true>,seq_contains<N,seq<S...>>>::type::value;
		};

		template <int N, typename T>
		struct seq_insert_one;
		template <int N, int ...S>
		struct seq_insert_one<N, seq<S...>>
		{
			typedef seq<N, S...> type;
		};

		template <int N, typename T>
		struct seq_remove_one;

		template <int N>
		struct seq_remove_one<N, seq<>>
		{
			typedef seq<> type;
		};

		template <int N, int F, int ... S>
		struct seq_remove_one<N, seq<F, S...>>
		{
			typedef typename if_c<N==F,
				seq<S...>,
				typename seq_insert_one<F, typename seq_remove_one<N, seq<S...>>::type>::type>::type type;
		};

		template <typename A, typename B>
		struct seq_diff;

		template <typename A>
		struct seq_diff<A, seq<>>
		{
			typedef A type;
		};

		template <typename A, int N, int ... D>
		struct seq_diff<A, seq<N, D...>>
		{
			typedef typename seq_diff<typename seq_remove_one<N, A>::type, seq<D...>>::type type;
		};

		template <typename A, typename B>
		struct seq_and
		{
			typedef typename seq_diff<A, typename seq_diff<A,B>::type>::type type;
		};
	}
}

