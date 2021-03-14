#pragma once

#include <vector>

#include "2d/CCNode.h"

NS_CC_BEGIN
class Sprite;
NS_CC_END

namespace cpg
{
	namespace input
	{
		using KeyCollectorSp = std::shared_ptr<class iKeyCollector>;
	}

	namespace input_test
	{
		class KeyMapConfigHelper;

		class KeyViewer : public cocos2d::Node
		{
		public:
			struct Config
			{
				bool bShowPivot = false;
				bool bShowBackground = false;
			};

		private:
			KeyViewer();

		public:
			static KeyViewer* create( const Config& config, const KeyMapConfigHelper& key_map_config_helper );

		private:
			bool init( const Config& config, const KeyMapConfigHelper& key_map_config_helper );

		public:
			void Setup( const cpg::input::KeyCollectorSp key_collector );

		private:
			struct KeyViewData
			{
				KeyViewData( int key_index, cocos2d::Sprite* sprite ) : mKeyIndex( key_index ), mSprite( sprite ) {}

				int mKeyIndex;
				cocos2d::Sprite* mSprite;
			};
			std::vector<KeyViewData> mKeyViews;
			cocos2d::Node* mFreeKeySprite;

			float mView_StartX;
			cocos2d::Size mView_Size;
		};
	}
}