#pragma once
#include "pure.h"

#ifdef PURE_USE_BOOST
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
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
	}
}

