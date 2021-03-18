#pragma once

#include "cpg_input_KeyCodeContainer.h"

namespace cpg_input
{
	class AllowedKeys
	{
	public:
		using Container = cpg_input::KeyCodeContainerT;

		AllowedKeys() = delete;

		static const Container load( const char* allowed_keys_file_name );
		static void save( Container _container, const char* allowed_keys_file_name );
	};
}