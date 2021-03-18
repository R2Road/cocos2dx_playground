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
		static const Container Load( const char* allowed_keys_file_name );
		static void Save( Container _container, const char* allowed_keys_file_name );
	};
}