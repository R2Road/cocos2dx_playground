#pragma once

#include "base/ccMacros.h"

namespace cpg
{
#pragma region Clamp from c++17
	template<class T, class Compare>
	constexpr const T& clamp( const T& v, const T& lo, const T& hi, Compare comp )
	{
		CCASSERT( !comp( hi, lo ), "" );
		return comp( v, lo ) ? lo : comp( hi, v ) ? hi : v;
	}

	template<class T>
	constexpr const T& clamp( const T& v, const T& lo, const T& hi )
	{
		return clamp( v, lo, hi, std::less<>() );
	}
#pragma endregion
}
