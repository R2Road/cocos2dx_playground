#pragma once

#include "2d/CCNode.h"

namespace step_flipflip
{
	namespace game
	{
		struct StageConfig;

		class CardSelectorNode : public cocos2d::Node
		{
		private:
			CardSelectorNode( const int width, const int height, const cocos2d::Size& card_area_size );

		public:
			static CardSelectorNode* create( const StageConfig& stage_config, const bool show_guide = false );

		private:
			bool init( const bool show_guide );

		public:
			int GetIndicatorX() const { return mCurrentX; }
			int GetIndicatorY() const { return mCurrentY; }
			void MoveIndicator( const int move_amount_x, const int move_amount_y, const bool bPlay_SFX );

		private:
			const int mWidth;
			const int mHeight;
			const cocos2d::Size mCardAreaSize;

			const cocos2d::Vec2 mPivotPosition;

			int mCurrentX;
			int mCurrentY;
			cocos2d::Node* mIndicator;
		};
	}
}
