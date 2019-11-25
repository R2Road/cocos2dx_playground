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
			struct KeyMapPiece
			{
				KeyMapPiece( const cocos2d::EventKeyboard::KeyCode _keycode, const int _idx ) : keycode( _keycode ), idx( _idx ) {}

				cocos2d::EventKeyboard::KeyCode keycode;
				int idx;
			};
			using KeyMapContainer = std::vector<KeyMapPiece>;

			using KeyStatusContainer = std::vector<bool>;

			Analyzer();

		public:
			static AnalyzerSp create();

		public:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode );

			const bool getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const;

		private:
			KeyMapContainer key_map_container;
			KeyStatusContainer key_status_container;
		};
	}
}