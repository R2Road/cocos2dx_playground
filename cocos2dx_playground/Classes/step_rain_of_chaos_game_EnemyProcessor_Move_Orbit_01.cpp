#include "step_rain_of_chaos_game_EnemyProcessor_Move_Orbit_01.h"

#include <algorithm>
#include <new>

#include "2d/CCNode.h"

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Move_Orbit_01::EnemyProcessor_Move_Orbit_01(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
			, const float distance_rate
		) : iEnemyProcessor( stage_config, owner_node, target_node )
			, mLimitTime( limit_time )
			, mDistance( stage_config.GetBulletGenerateRadiusMax() * distance_rate )

			, mStartPosition()
			, mMove()

			, mElapsedTime( 0.f )
		{}

		EnemyProcessorUp EnemyProcessor_Move_Orbit_01::Create(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
			, const float distance_rate
		)
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Move_Orbit_01( stage_config, owner_node, target_node, limit_time, distance_rate ) );
			ret->init();
			return ret;
		}

		void EnemyProcessor_Move_Orbit_01::Enter()
		{
			mStartPosition = mOwnerNode->getPosition();

			auto center_to_start = mStartPosition - mStageConfig.GetCenter();
			center_to_start.normalize();
			center_to_start.scale( mDistance );

			const auto target_position = center_to_start + mStageConfig.GetCenter();

			mMove = target_position - mStartPosition;

			mElapsedTime = 0.f;
		}

		bool EnemyProcessor_Move_Orbit_01::Update( const float delta_time )
		{
			if( mLimitTime <= mElapsedTime )
			{
				return false;
			}

			mElapsedTime += delta_time;

			float rate = mElapsedTime / mLimitTime;
			rate = std::min( 1.f, rate );

			mOwnerNode->setPosition( mStartPosition + ( mMove * rate ) );

			return mLimitTime > mElapsedTime;
		}
	}
}
