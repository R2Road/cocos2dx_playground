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
		private:
			KeyViewer();

		public:
			static KeyViewer* create( const KeyMapConfigHelper& key_map_config_helper );

			bool init( const KeyMapConfigHelper& key_map_config_helper );

			void setup( const cpg::input::KeyCollectorSp key_collector );

		private:
			struct KeyViewData
			{
				KeyViewData( int key_index, cocos2d::Sprite* sprite ) : mKeyIndex( key_index ), mSprite( sprite ) {}

				int mKeyIndex;
				cocos2d::Sprite* mSprite;
			};
			std::vector<KeyViewData> mKeyViews;
			float mView_StartX;
			cocos2d::Size mView_Size;
		};
	}
}