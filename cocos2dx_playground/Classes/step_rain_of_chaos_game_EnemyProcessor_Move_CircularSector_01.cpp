#include "step_rain_of_chaos_game_EnemyProcessor_Move_CircularSector_01.h"

#include <algorithm>
#include <new>

#include "2d/CCNode.h"

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Move_CircularSector_01::EnemyProcessor_Move_CircularSector_01(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
			, const bool direction
			, const float degree_per_cycle
		) : iEnemyProcessor( stage_config, owner_node, target_node )
			, mLimitTime( limit_time )
			, mRadianPerCycle( ( CC_DEGREES_TO_RADIANS( degree_per_cycle ) * ( direction ? 1 : -1 ) ) )

			, mMove()

			, mElapsedTime( 0.f )
		{}

		EnemyProcessorUp EnemyProcessor_Move_CircularSector_01::Create(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
			, const bool direction
			, const float degree_per_cycle
		)
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Move_CircularSector_01( stage_config, owner_node, target_node, limit_time, direction, degree_per_cycle ) );
			ret->init();
			return ret;
		}

		void EnemyProcessor_Move_CircularSector_01::Enter()
		{
			mMove = mOwnerNode->getPosition() - mStageConfig.GetCenter();

			mElapsedTime = 0.f;
		}

		bool EnemyProcessor_Move_CircularSector_01::Update( const float delta_time )
		{
			if( mLimitTime <= mElapsedTime )
			{
				return false;
			}

			mElapsedTime += delta_time;

			float rate = mElapsedTime / mLimitTime;
			rate = std::min( 1.f, rate );

			Vec2 temp_move = mMove;
			temp_move.rotate( Vec2::ZERO, mRadianPerCycle * rate );

			mOwnerNode->setPosition( mStageConfig.GetCenter() + temp_move );

			return mLimitTime > mElapsedTime;
		}
	}
}
