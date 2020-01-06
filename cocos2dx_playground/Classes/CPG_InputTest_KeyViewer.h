#pragma once

#include <vector>

#include "cocos2d.h"

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
			static KeyViewer* create( const KeyMapConfigHelper& _key_map_config_helper );

			bool init( const KeyMapConfigHelper& _key_map_config_helper );

			void setup( const cpg::input::KeyCollectorSp key_collector );

		private:
			struct KeyViewData
			{
				KeyViewData( int _key_index, cocos2d::Sprite* _sprite ) : key_index( _key_index ), sprite( _sprite ) {}

				int key_index;
				cocos2d::Sprite* sprite;
			};
			std::vector<KeyViewData> key_views;
			float view_start_x;
			cocos2d::Size view_size;
		};
	}
}