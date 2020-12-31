#pragma once

#include <array>

#include "2d/CCNode.h"

#include "step_clickclick_game_Constant.h"

NS_CC_BEGIN
	class Action;
NS_CC_END

namespace step_clickclick
{
	namespace game
	{
		class EffectViewNode : public cocos2d::Node
		{
		private:
			EffectViewNode();

		public:
			~EffectViewNode();

			static EffectViewNode* create();

		private:
			bool init() override;

		public:
			void PlayEffect( const eEffectIndex effect_index );

		private:
			cocos2d::Node* mSpriteNode;
			std::array<cocos2d::Action*, 3u> mAnimationActions;
		};
	}
}
