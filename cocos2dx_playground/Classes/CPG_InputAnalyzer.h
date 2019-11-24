#pragma once

#include <vector>
#include <memory>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		using AnalyzerSp = std::shared_ptr<class Analyzer>;

		class Analyzer
		{
		private:
			struct KeyStatus
			{
				KeyStatus() : keycode( cocos2d::EventKeyboard::KeyCode::KEY_NONE ), status( false ) {}
				KeyStatus( cocos2d::EventKeyboard::KeyCode _keycode ) : keycode( _keycode ), status( false ) {}

				cocos2d::EventKeyboard::KeyCode keycode;
				bool status;
			};
			using KeyStatusContainer = std::vector<KeyStatus>;

			Analyzer();

		public:
			static AnalyzerSp create();

		public:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode );

			const bool getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const;

		private:
			KeyStatusContainer key_status_container;
		};
	}
}