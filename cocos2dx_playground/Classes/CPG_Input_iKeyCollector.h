#pragma once

#include <memory>

#include "cocos/base/CCEventKeyboard.h"

namespace cpg
{
	namespace input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;
		class KeyCodeCollector;
		using KeyCollectorSp = std::shared_ptr<class iKeyCollector>;

		class iKeyCollector
		{
		protected:
			iKeyCollector( const KeyMapSp& key_map_container );
			virtual ~iKeyCollector() {}

		public:
			virtual void collect( const KeyCodeCollector& key_code_collector ) = 0;
			virtual void update_forHistory() = 0;

			virtual const bool getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const = 0;
			virtual const bool getKeyStatus( const int target_key_index ) const = 0;
			virtual const bool hasChanged() const = 0;

		protected:
			const KeyMapSp mKeyMapContainer;
		};
	}
}