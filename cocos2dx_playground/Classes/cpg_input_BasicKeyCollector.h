#pragma once

#include <array>
#include <bitset>

#include "cpg_input_iKeyCollector.h"

namespace step_rain_of_chaos
{
	namespace input
	{
		class KeyCodeCollector;
	}
}

namespace cpg_input
{
	class BasicKeyCollector : public iKeyCollector
	{
	private:
		using KeyStatusContainer = std::bitset<31u>;
		using KeyHistory = std::array<KeyStatusContainer, 10u>;

		BasicKeyCollector( const KeyMapSp& key_map_container );

	public:
		static KeyCollectorSp create( const KeyMapSp& key_map_container );

	public:
		void collect( const step_rain_of_chaos::input::KeyCodeCollector& key_code_collector ) override;
		void update_forHistory() override;

		bool getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const override;
		bool getKeyStatus( const int target_key_index ) const override;
		bool hasChanged() const override;

	private:
		KeyHistory mKeyHistory;
		KeyHistory::iterator mCurrent_KeyStatus_Container;
		KeyHistory::iterator mLast_KeyStatus_Container;
	};
}