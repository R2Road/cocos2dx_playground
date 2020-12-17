#pragma once

#include "2d/CCNode.h"

namespace step_flipflip
{
	namespace game
	{
		class CardSelectorNode : public cocos2d::Node
		{
		private:
			CardSelectorNode( const int width, const int height );

		public:
			static CardSelectorNode* create( const int width, const int height, const bool show_guide = false );

		private:
			bool init( const bool show_guide );

		public:
			void MoveIndicator( const int move_amount_x, const int move_amount_y );

		private:
			const int mWidth;
			const int mHeight;
			const cocos2d::Vec2 mPivotPosition;

			int mCurrentX;
			int mCurrentY;
			cocos2d::Node* mIndicator;
		};
	}
}
