#pragma once

#include <functional>
#include <vector>

#include "2d/CCNode.h"
#include "ui/UIWidget.h"

#include "cpg_GridIndexConverter.h"
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
		class Stage;

		class StageView : public cocos2d::Node
		{
		public:
			using OnPannelCallback = std::function<void( int )>;

		private:
			class PannelView
			{
			public:
				PannelView(
					cocos2d::Node* const pannel_node, cocos2d::Sprite* const view_node, cocos2d::Label* const label_node
					, cocos2d::Sprite* const effect_node
					, cocos2d::Action* const increase_effect_action
					, cocos2d::Action* const decrease_effect_action
					, cocos2d::Action* const die_effect_action
				);

				void Init( eBlockType type, const int life );
				void SetVisible( const bool visible );
				void Update( const int last_life, const int current_life );

			private:
				cocos2d::Node* const mPannelNode;
				cocos2d::Sprite* const mViewNode;
				cocos2d::Label* const mLabelNode;
				cocos2d::Sprite* const mEffectNode;
				cocos2d::Action* const mIncreaseEffectAction;
				cocos2d::Action* const mDecreaseEffectAction;
				cocos2d::Action* const mDieEffectAction;
			};

			StageView( const int width, const int height, const OnPannelCallback& on_pannel_callback );

		public:
			static StageView* create( const int width, const int height, const OnPannelCallback& on_pannel_callback );

			bool init() override;
			void Setup( const Stage& stage_data );
			void UpdateBlock( const int linear_index, const int last_life, const int current_life );

		private:
			void onBlock( Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

		private:
			const int mStageWidth;
			const int mStageHeight;
			const cpg::GridIndexConverter mGridIndexConverter;
			std::vector<PannelView> PannelViews;

			const OnPannelCallback mOnPannelCallback;
		};
	}
}