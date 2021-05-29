#pragma once

#include <sstream>

namespace cpg
{
	template<class _Elem, class _Traits>
	inline std::basic_ostream<_Elem, _Traits>& linefeed( std::basic_ostream<_Elem, _Traits>& _Ostr )
	{
		// insert newline
		_Ostr.put( _Ostr.widen( '\n' ) );
		return ( _Ostr );
	}

	template<class _Elem, class _Traits>
	inline std::basic_ostream<_Elem, _Traits>& split( std::basic_ostream<_Elem, _Traits>& _Ostr )
	{
		_Ostr << r2::linefeed << "====================================================================" << r2::linefeed << r2::linefeed;
		return ( _Ostr );
	}
}
