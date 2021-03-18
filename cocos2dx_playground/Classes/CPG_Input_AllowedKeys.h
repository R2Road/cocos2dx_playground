#pragma once

#include "cpg_input_KeyCodeContainer.h"

namespace cpg_input
{
	class AllowedKeys
	{
	public:
		using Container = cpg_input::KeyCodeContainerT;

	private:
		AllowedKeys() = delete;

	public:
		static void Load( Container& container, const char* allowed_keys_file_name );
		static void Save( const Container& container, const char* allowed_keys_file_name );
	};
}