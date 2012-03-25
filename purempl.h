#pragma once
#include "pure.h"

#ifdef PURE_USE_BOOST
#include <boost/mpl/if.hpp>
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
		template <int...> struct seq{};

		template <int N, int ...S>
			struct count : count<N-1, N-1, S...> {};

		template <int ... S>
			struct count<0, S...>
			{
				typedef seq<S...> type;
			};
	}
}

