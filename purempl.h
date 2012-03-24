#pragma once
#include "pure.h"
#ifdef PURE_USE_BOOST
#include <boost/mpl/if.hpp>
namespace pure
{
	using namespace boost;
}
#else
#error boost required currently.
#endif
