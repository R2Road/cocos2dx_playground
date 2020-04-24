#pragma once

#include "2d/CCNode.h"
#include "ui/UIWidget.h"
#include "math/CCGeometry.h"

#include "step_clickclick_game_Constant.h"

NS_CC_BEGIN
	class Label;
	class Sprite;
	class Action;
NS_CC_END

namespace step_clickclick
{
	namespace game
	{
		class BlockView : public cocos2d::Node
		{
		public:
			using OnBlockCallback = std::function<void( int )>;

		private:
			BlockView( const OnBlockCallback& on_block_callback );

		public:
			~BlockView();

			static BlockView* create( const int linear_index, const cocos2d::Size block_size, const OnBlockCallback& on_block_callback );

			bool init( const int linear_index, const cocos2d::Size block_size );

			void Reset( eBlockType type, const int life );
			void SetVisible( const bool visible );
			void UpdateLife( const int last_life, const int current_life );

		private:
			void onBlock( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

		private:
			cocos2d::Node* mButtonNode;
			cocos2d::Sprite* mViewNode;
			cocos2d::Label* mLabelNode;
			cocos2d::Sprite* mEffectNode;
			cocos2d::Action* mIncreaseEffectAction;
			cocos2d::Action* mDecreaseEffectAction;
			cocos2d::Action* mDieEffectAction;

			const OnBlockCallback mOnBlockCallback;
		};
	}
}
