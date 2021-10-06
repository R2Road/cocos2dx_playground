#pragma once

#include <memory>

#include "cocos/base/CCEventKeyboard.h"

namespace step_rain_of_chaos
{
	namespace input
	{
		class KeyCodeCollector;
	}
}

namespace cpg_input
{
	using KeyMapSp = std::shared_ptr<class KeyMap>;
	using KeyCollectorSp = std::shared_ptr<class iKeyCollector>;

	class iKeyCollector
	{
	protected:
		iKeyCollector( const KeyMapSp& key_map_container );
		virtual ~iKeyCollector() {}

	public:
		virtual void collect( const step_rain_of_chaos::input::KeyCodeCollector& key_code_collector ) = 0;
		virtual void update_forHistory() = 0;

		virtual bool getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const = 0;
		virtual bool getKeyStatus( const int target_key_index ) const = 0;
		virtual bool hasChanged() const = 0;

	protected:
		const KeyMapSp mKeyMapContainer;
	};
}