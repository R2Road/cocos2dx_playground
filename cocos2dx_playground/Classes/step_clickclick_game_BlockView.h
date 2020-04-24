#pragma once

#include "2d/CCNode.h"

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
		class BlockView
		{
		public:
			BlockView(
				cocos2d::Node* const button_node, cocos2d::Sprite* const view_node, cocos2d::Label* const label_node
				, cocos2d::Sprite* const effect_node
				, cocos2d::Action* const increase_effect_action
				, cocos2d::Action* const decrease_effect_action
				, cocos2d::Action* const die_effect_action
			);

			void Init( eBlockType type, const int life );
			void SetVisible( const bool visible );
			void Update( const int last_life, const int current_life );

		private:
			cocos2d::Node* const mButtonNode;
			cocos2d::Sprite* const mViewNode;
			cocos2d::Label* const mLabelNode;
			cocos2d::Sprite* const mEffectNode;
			cocos2d::Action* const mIncreaseEffectAction;
			cocos2d::Action* const mDecreaseEffectAction;
			cocos2d::Action* const mDieEffectAction;
		};
	}
}
