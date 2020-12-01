#pragma once

#include "2d/CCNode.h"

#include "step_flipflip_game_Constant.h"

NS_CC_BEGIN
	class Sprite;
	class SpriteFrame;
NS_CC_END

namespace step_flipflip
{
	namespace game
	{
		class CardViewNode : public cocos2d::Node
		{
		private:
			CardViewNode();

		public:
			static CardViewNode* create( const eCardType card_type );

		private:
			bool init( const eCardType card_type );

		public:
			void Flip();

		private:
			bool mbFrontSide;

			cocos2d::Sprite* mView;
			cocos2d::SpriteFrame* mBackSideSpriteFrame;
			cocos2d::SpriteFrame* mFrontSideSpriteFrame;
		};
	}
}
