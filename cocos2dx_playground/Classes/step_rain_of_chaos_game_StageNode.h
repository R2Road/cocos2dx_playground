#pragma once

#include <functional>
#include <vector>

#include "2d/CCNode.h"

#include "step_mole_CircleCollisionComponentConfig.h"
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
		using BulletManagerUp = std::unique_ptr<class BulletManager>;

		class StageNode : public cocos2d::Node
		{
		public:
			using BulletProcessExitCallback = std::function<void( int )>;
			using PlayerCollisionCallback = std::function<void()>;
			struct DebugConfig
			{
				bool bShowPivot = false;
				bool bShowLabel_StageArea = false;
				bool bShowGuide_BulletLifeArea = false;
				bool bShowGuide_BulletGenerateArea = false;
			};

		private:
			StageNode(
				const StageConfig stage_config
				, const DebugConfig debug_config
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
			);

		public:
			static StageNode* create(
				const StageConfig stage_config
				, const DebugConfig debug_config
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
				, const int bullet_count
			);

		private:
			bool init( const int bullet_count );

			cocos2d::Node* MakeBullet(
				const int index
				, const BulletProcessExitCallback& target_rest_callback
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
				, const bool bShowPivot
			);

			void update4Collision( float dt );

		public:
			void AddPlayer( cocos2d::Node* player_node );
			void PlayerMoveRequest( const cocos2d::Vec2& move_vector );
			void SetPlayerCollisionCallback( const PlayerCollisionCallback& player_collision_callback ) { mPlayerCollisionCallback = player_collision_callback; }

			void AddEnemy( cocos2d::Node* const enemy_node );

			void RequestGenerateBullet( const int amount = 1 );
			void RequestBulletAction( const cocos2d::Vec2 start_position, const cocos2d::Vec2 move_direction );

		private:
			const StageConfig mStageConfig;
			const DebugConfig mDebugConfig;
			const step_mole::CircleCollisionComponentConfig mCircleCollisionComponentConfig;

			BulletManagerUp mBulletManager;
			std::vector<step_rain_of_chaos::game::BulletLifeComponent*> mBulletLifeComponentList;
			std::vector<step_mole::CircleCollisionComponent*> mCollisionComponentList;
			int mBulletCount;

			cocos2d::Node* mPlayerNode;
			step_mole::CircleCollisionComponent* mPlayerCircleCollisionComponent;
			PlayerCollisionCallback mPlayerCollisionCallback;

			cocos2d::Node* mEnemyNode;
			step_mole::CircleCollisionComponent* mEnemyCircleCollisionComponent;
		};
	}
}
