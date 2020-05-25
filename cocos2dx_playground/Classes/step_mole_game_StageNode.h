#pragma once

#include <functional>
#include <vector>

#include "2d/CCNode.h"
#include "ui/UIWidget.h"
#include "math/CCGeometry.h"

#include "step_mole_game_StageConfig.h"

namespace step_mole
{
	class ObjectComponent;
	class CircleCollisionComponent;

	struct CircleCollisionComponentConfig;

	namespace game
	{
		struct StageNodeConfig
		{
			bool bShowPivot = false;
			bool bShowBackgroundGuide = false;
		};

		class StageNode : public cocos2d::Node
		{
		public:
			using TargetProcessExitCallback = std::function<void( int )>;

		private:
			StageNode( const StageConfig stage_config );

		public:
			static StageNode* create(
				const StageConfig stage_config
				, const TargetProcessExitCallback& target_rest_callback
				, const StageNodeConfig stage_view_config
				, const CircleCollisionComponentConfig& circle_collision_component_config
			);

		private:
			bool init( const TargetProcessExitCallback& target_rest_callback, const StageNodeConfig stage_view_config, const CircleCollisionComponentConfig& circle_collision_component_config );

			cocos2d::Node* MakeObject(
				const int object_tag
				, const cocos2d::Vec2 object_position
				, const int defalut_view_type
				, const TargetProcessExitCallback& target_rest_callback
				, const CircleCollisionComponentConfig& circle_collision_component_config
			);
			void onStageClick( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type );

		public:
			void RequestAction( const std::size_t object_index, const float life_time );

		private:
			const StageConfig mStageConfig;

			std::vector<ObjectComponent*> mObjectComponentList;
			std::vector<CircleCollisionComponent*> mCollisionComponentList;
		};
	}
}
