#include "step_rain_of_chaos_game_EnemyProcessor_Move_CircularSector_Random_02.h"

#include <algorithm>
#include <new>

#include "2d/CCNode.h"

#include "cpg_Random.h"
#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Move_CircularSector_Random_02::EnemyProcessor_Move_CircularSector_Random_02(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
			, const float degree_per_cycle_min
			, const float degree_per_cycle_max
		) : iEnemyProcessor( stage_config, owner_node, target_node )
			, mLimitTime( limit_time )
			, mRadianPerCycle_MIN( ( CC_DEGREES_TO_RADIANS( degree_per_cycle_min ) ) )
			, mRadianPerCycle_MAX( ( CC_DEGREES_TO_RADIANS( degree_per_cycle_max ) ) )

			, mRadianPerCycle( 0.f )
			, mPivot()

			, mElapsedTime( 0.f )
		{}

		EnemyProcessorUp EnemyProcessor_Move_CircularSector_Random_02::Create(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
			, const float degree_per_cycle_min
			, const float degree_per_cycle_max
		)
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Move_CircularSector_Random_02( stage_config, owner_node, target_node, limit_time, degree_per_cycle_min, degree_per_cycle_max ) );
			ret->init();
			return ret;
		}

		void EnemyProcessor_Move_CircularSector_Random_02::Enter()
		{
			mRadianPerCycle = cpg::Random::GetFloat( mRadianPerCycle_MIN, mRadianPerCycle_MAX ) * ( cpg::Random::GetBool() ? 1 : -1 );
			mPivot = mOwnerNode->getPosition() - mStageConfig.GetCenter();

			mElapsedTime = 0.f;
		}

		bool EnemyProcessor_Move_CircularSector_Random_02::Update( const float delta_time )
		{
			if( mLimitTime <= mElapsedTime )
			{
				return false;
			}

			mElapsedTime += delta_time;

			float rate = mElapsedTime / mLimitTime;
			rate = std::min( 1.f, rate );

			Vec2 temp_move = mPivot;
			temp_move.rotate( Vec2::ZERO, mRadianPerCycle * rate );

			mOwnerNode->setPosition( mStageConfig.GetCenter() + temp_move );

			return mLimitTime > mElapsedTime;
		}
	}
}
