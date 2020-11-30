#pragma once

#include "2d/CCNode.h"

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
		public:
			enum class eCardType
			{
				A
				,B
				,C
				,D
				,E

				, SIZE
			};

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
