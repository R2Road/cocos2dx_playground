#pragma once

#include <functional>
#include <vector>

#include "2d/CCNode.h"

#include "step_rain_of_chaos_game_StageConfig.h"

namespace step_mole
{
	class CircleCollisionComponent;
	struct CircleCollisionComponentConfig;
}

namespace step_rain_of_chaos
{
	namespace game
	{
		class BulletLifeComponent;

		class StageNode : public cocos2d::Node
		{
		public:
			using BulletProcessExitCallback = std::function<void( int )>;
			struct DebugConfig
			{
				bool bShowPivot = false;
				bool bShowAreaGuide = false;
			};

		private:
			StageNode( const StageConfig stage_config, const int bullet_count );

		public:
			static StageNode* create(
				const StageConfig stage_config, const DebugConfig debug_config
				, const int bullet_count
				, const BulletProcessExitCallback& bullet_process_exit_callback
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
			);

		private:
			bool init(
				const DebugConfig debug_config
				, const int bullet_count
				, const BulletProcessExitCallback& bullet_process_exit_callback
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
			);

			cocos2d::Node* MakeBullet(
				const int index
				, const BulletProcessExitCallback& target_rest_callback
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
				, const bool bShowPivot
			);

		public:
			void RequestAction( const std::size_t bullet_index, const cocos2d::Vec2 start_position, const cocos2d::Vec2 move_direction );

		private:
			const StageConfig mStageConfig;

			std::vector<step_rain_of_chaos::game::BulletLifeComponent*> mBulletLifeComponentList;
			std::vector<step_mole::CircleCollisionComponent*> mCollisionComponentList;

			int mBulletCount;
		};
	}
}
