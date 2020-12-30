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
		class EffectView : public cocos2d::Node
		{
		private:
			EffectView();

		public:
			~EffectView();

			static EffectView* create();

		private:
			bool init() override;

		public:
			void PlayEffect( const eAnimationIndex animation_index );

		private:
			cocos2d::Node* mEffectNode;
			std::array<cocos2d::Action*, 3u> mAnimationActions;
		};
	}
}
