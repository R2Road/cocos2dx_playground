#pragma once

#include "CPG_Input_KeyCodeContainer.h"

namespace cpg
{
	namespace input
	{
		class AllowedKeys
		{
		public:
			using Container = step_rain_of_chaos::input::KeyCodeContainerT;

			AllowedKeys() = delete;

			static const Container load( const char* allowed_keys_file_name );
			static void save( Container _container, const char* allowed_keys_file_name );
		};
	}
}