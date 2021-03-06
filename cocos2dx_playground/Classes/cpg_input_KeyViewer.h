#pragma once

#include <vector>

#include "2d/CCNode.h"

NS_CC_BEGIN
class Sprite;
NS_CC_END

namespace cpg_input
{
	class iKeyCollector;
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
		struct KeyViewData
		{
			KeyViewData( int key_index, cocos2d::Sprite* sprite_node ) : KeyIndex( key_index ), SpriteNode( sprite_node ) {}

			int KeyIndex;
			cocos2d::Sprite* SpriteNode;
		};

		KeyViewer();

	public:
		static KeyViewer* create( const Config& config, const KeyMapConfigHelper& key_map_config_helper );

	private:
		bool init( const Config& config, const KeyMapConfigHelper& key_map_config_helper );

	public:
		void Setup( const iKeyCollector& key_collector );

	private:
		std::vector<KeyViewData> mKeyViews;
		cocos2d::Node* mFreeKeySprite;

		cocos2d::Size mKeySize;
	};
}