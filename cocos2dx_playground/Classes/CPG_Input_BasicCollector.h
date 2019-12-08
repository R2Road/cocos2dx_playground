#pragma once

#include <array>
#include <memory>
#include <bitset>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;
		using BasicCollectorSp = std::shared_ptr<class BasicCollector>;
		class KeyCodeCollector;

		class BasicCollector
		{
		private:
			using KeyStatusContainer = std::bitset<31u>;
			using KeyHistory = std::array<KeyStatusContainer, 10u>;

			BasicCollector( const KeyMapSp& _key_map_container );

		public:
			static BasicCollectorSp create( const KeyMapSp& _key_map_container );

		public:
			void collect( const KeyCodeCollector& _key_code_collector );
			void update();

			const bool getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const;
			const bool getKeyStatus( const int target_key_index ) const;
			const int getKeyStatusPackage() const { return current_key_status_container->to_ulong(); }
			const bool hasChanged() const;

		private:
			const KeyMapSp key_map_container;
			KeyHistory key_history;
			KeyHistory::iterator current_key_status_container;
			KeyHistory::iterator last_key_status_container;
		};
	}
}