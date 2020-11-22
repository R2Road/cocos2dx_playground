#pragma once

namespace cpg
{
	class StringTable
	{
	private:
		StringTable() = delete;

	public:
		static const char* GetFontPath();
	};
}