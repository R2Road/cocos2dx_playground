#pragma once

#include "2d/CCNode.h"

#include "step_flipflip_game_Constant.h"

NS_CC_BEGIN
	class Action;
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
			~CardViewNode();

			static CardViewNode* create( const eCardType card_type );

		private:
			bool init( const eCardType card_type );

		public:
			bool IsOpen() const { return mbFrontSide; }
			bool IsFlipping() const;

			void Flip();

		private:
			bool mbFrontSide;

			cocos2d::Node* mView;
			cocos2d::Action* mAction4FrontSide;
			cocos2d::Action* mAction4BackSide;
		};
	}
}
