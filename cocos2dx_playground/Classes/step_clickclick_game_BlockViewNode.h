#pragma once

#include "2d/CCNode.h"
#include "ui/UIWidget.h"

#include "step_clickclick_game_Constant.h"

NS_CC_BEGIN
	class Sprite;
	class Label;
NS_CC_END

namespace step_clickclick
{
	namespace game
	{
		class BlockViewNode : public cocos2d::Node
		{
		public:
			using OnBlockCallback = std::function<void( int )>;

		private:
			BlockViewNode( const int index, const OnBlockCallback& on_block_callback );

		public:
			static BlockViewNode* create( const int index, const cocos2d::Size block_size, const OnBlockCallback& on_block_callback );

		private:
			bool init( const cocos2d::Size block_size );

		public:
			void Reset( const eBlockType type, const int life );
			void UpdateLife( const int current_life );

		private:
			void onBlock( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

		private:
			const int mIndex;

			cocos2d::Node* mButtonNode;
			cocos2d::Sprite* mViewNode;
			cocos2d::Label* mLifeLabel;

			const OnBlockCallback mOnBlockCallback;
		};
	}
}
