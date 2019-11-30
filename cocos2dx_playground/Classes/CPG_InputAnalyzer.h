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
		using AnalyzerSp = std::shared_ptr<class Analyzer>;

		class Analyzer
		{
		private:
			using KeyStatusContainer = std::bitset<31u>;
			using KeyHistory = std::array<KeyStatusContainer, 10u>;

			Analyzer( const KeyMapSp& _key_map_container );

		public:
			static AnalyzerSp create( const KeyMapSp& _key_map_container );

		public:
			void update();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode );

			const bool getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const;
			const bool getKeyStatus( const int target_key_index ) const;
			const int getKeyStatusPackage() const { return current_key_status_container->to_ulong(); }

		private:
			const KeyMapSp key_map_container;
			KeyHistory key_history;
			KeyHistory::iterator current_key_status_container;
		};
	}
}