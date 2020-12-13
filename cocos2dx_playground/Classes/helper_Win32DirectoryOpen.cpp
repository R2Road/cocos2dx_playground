#include "helper_Win32DirectoryOpen.h"

#include <codecvt>
#include <cstring>
#include <locale>
#include <string>

#include "base/ccMacros.h"

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
#include "shellapi.h"
#endif

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
		const auto utf8_path_length = strlen( utf8_path );

		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		const std::wstring temp = conv.from_bytes( utf8_path, utf8_path + utf8_path_length ); // nrvo?

		ShellExecute( NULL, L"open", temp.c_str(), NULL, NULL, SW_SHOWNORMAL );
#endif
	}
}