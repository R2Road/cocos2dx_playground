#pragma once

#include <string>

#include "2d/CCNode.h"

NS_CC_BEGIN
class Sprite;
class SpriteBatchNode;
NS_CC_END

namespace step_defender
{
	namespace game
	{
		class TileMapNode : public cocos2d::Node
		{
		public:
			struct Config
			{
				int MapWidth = 10;
				int MapHeight = 10;

				int TileWidth = 32;
				int TileHeight = 32;

				std::string TexturePath;
			};

		private:
			TileMapNode( const Config& config );

		public:
			~TileMapNode();

			static TileMapNode* create( const Config& config );

		private:
			bool init() override;

		public:
			void Reset();

		private:
			const Config mConfig;

			cocos2d::SpriteBatchNode* mSpriteBatchNode;
			cocos2d::Sprite* mReusedSprite;
		};
	}
}
