#pragma once

#include <array>
#include <bitset>

#include "CPG_Input_iKeyCollector.h"

namespace CPG
{
	namespace Input
	{
		class KeyCodeCollector;

		class BasicCollector : public iKeyCollector
		{
		private:
			using KeyStatusContainer = std::bitset<31u>;
			using KeyHistory = std::array<KeyStatusContainer, 10u>;

			BasicCollector( const KeyMapSp& _key_map_container );

		public:
			static KeyCollectorSp create( const KeyMapSp& _key_map_container );

		public:
			void collect( const KeyCodeCollector& _key_code_collector ) override;
			void update_forHistory() override;

			const bool getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const override;
			const bool getKeyStatus( const int target_key_index ) const override;
			const bool hasChanged() const override;

		private:
			KeyHistory key_history;
			KeyHistory::iterator current_key_status_container;
			KeyHistory::iterator last_key_status_container;
		};
	}
}