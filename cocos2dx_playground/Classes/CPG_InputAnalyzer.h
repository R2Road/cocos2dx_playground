#pragma once

#include <vector>
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

			Analyzer( const KeyMapSp& _key_map_container );

		public:
			static AnalyzerSp create( const KeyMapSp& _key_map_container );

		public:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode );

			const bool getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const;

		private:
			const KeyMapSp key_map_container;
			KeyStatusContainer key_status_container;
		};
	}
}