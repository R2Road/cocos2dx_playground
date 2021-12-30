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



	template<class _Elem, class _Traits>
	inline std::basic_ostream<_Elem, _Traits>& tab( std::basic_ostream<_Elem, _Traits>& _Ostr )
	{
		_Ostr.put( _Ostr.widen( '\t' ) );
		return ( _Ostr );
	}
	template<class _Elem, class _Traits>
	inline std::basic_ostream<_Elem, _Traits>& tab2( std::basic_ostream<_Elem, _Traits>& _Ostr )
	{
		_Ostr << cpg::tab << cpg::tab;
		return ( _Ostr );
	}
	template<class _Elem, class _Traits>
	inline std::basic_ostream<_Elem, _Traits>& tab3( std::basic_ostream<_Elem, _Traits>& _Ostr )
	{
		_Ostr << cpg::tab << cpg::tab << cpg::tab;
		return ( _Ostr );
	}



	template<class _Elem, class _Traits>
	inline std::basic_ostream<_Elem, _Traits>& blank5( std::basic_ostream<_Elem, _Traits>& _Ostr )
	{
		_Ostr << "     ";
		return ( _Ostr );
	}
}
