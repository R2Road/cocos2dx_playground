#pragma once

#include <functional>
#include <array>

#include "2d/CCNode.h"
#include "ui/UIWidget.h"

#include "step_mole_game_StageConfig.h"

namespace step_mole
{
	class AnimationComponent;

	namespace game
	{
		struct HittingNodeConfig
		{
			bool bShowPivot = false;
			bool bShowNodeGuide = false;
		};

		class HittingNode : public cocos2d::Node
		{
		public:
			using HittingCallback = std::function<void( const float target_x, const float target_y )>;

		private:
			HittingNode( const HittingCallback& hitting_callback );

		public:
			static HittingNode* create( const StageConfig& stage_config, const HittingNodeConfig& hitting_node_config, const HittingCallback& hitting_callback );

		private:
			bool init( const StageConfig& stage_config, const HittingNodeConfig& hitting_node_config );

			AnimationComponent* getEffectAnimationComponent();
			void onStageClick( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

		private:
			const HittingCallback mHittingCallback;

			enum { CachedEffectCount = 3 };
			std::array<AnimationComponent*, CachedEffectCount> mEffectAnimationComponents;
			std::array<AnimationComponent*, CachedEffectCount>::iterator mEffectAnimationComponentIndicator;
		};
	}
}
