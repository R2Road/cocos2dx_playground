#pragma once

#include <codecvt>
#include <locale>
#include <string>

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	#include "shellapi.h"
#endif

#include "platform/CCFileUtils.h"

namespace helper
{
	bool isEnableWin32DirectoryOpen()
	{
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
		return true;
#elif
		return false;
#endif
	}

	void Win32DirectoryOpen( const char* utf8_path )
	{
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
		std::wstring temp;
		if( temp.empty() )
		{
			const std::string writable_path( utf8_path );

			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			temp = conv.from_bytes( writable_path.c_str(), writable_path.c_str() + writable_path.length() - 1 );
		}

		ShellExecute( NULL, L"open", temp.c_str(), NULL, NULL, SW_SHOWNORMAL );
#endif
	}
}
