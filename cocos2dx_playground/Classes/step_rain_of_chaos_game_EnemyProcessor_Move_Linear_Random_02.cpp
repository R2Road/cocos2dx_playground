#include "step_rain_of_chaos_game_EnemyProcessor_Move_Linear_Random_02.h"

#include <algorithm>
#include <new>

#include "2d/CCNode.h"

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Move_Linear_Random_02::EnemyProcessor_Move_Linear_Random_02(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
			, const float degree_per_cycle_min
			, const float degree_per_cycle_max
		) : iEnemyProcessor( stage_config, owner_node, target_node )
			, mLimitTime( limit_time )
			, mRadianPerCycle_MIN( CC_DEGREES_TO_RADIANS( degree_per_cycle_min ) )
			, mRadianPerCycle_MAX( CC_DEGREES_TO_RADIANS( degree_per_cycle_max ) )

			, mRadianPerCycle( 0.f )
			, mStartPosition()
			, mMove()

			, mElapsedTime( 0.f )
		{}

		EnemyProcessorUp EnemyProcessor_Move_Linear_Random_02::Create(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
			, const float degree_per_cycle_min
			, const float degree_per_cycle_max
		)
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Move_Linear_Random_02( stage_config, owner_node, target_node, limit_time, degree_per_cycle_min, degree_per_cycle_max ) );
			ret->init();
			return ret;
		}

		void EnemyProcessor_Move_Linear_Random_02::Enter()
		{
			std::random_device rd;
			std::mt19937 randomEngine( rd() );
			std::uniform_real_distribution<> dist( mRadianPerCycle_MIN, mRadianPerCycle_MAX );
			std::uniform_int_distribution<> dist2( 0, 1 );
			mRadianPerCycle = dist( randomEngine ) * ( dist2( randomEngine ) ? 1 : -1 );

			mStartPosition = mOwnerNode->getPosition();

			auto center_to_start = mStartPosition - mStageConfig.GetCenter();
			auto center_to_end = center_to_start;
			center_to_end.rotate( Vec2::ZERO, mRadianPerCycle );

			mMove = center_to_end - center_to_start;

			mElapsedTime = 0.f;
		}

		bool EnemyProcessor_Move_Linear_Random_02::Update( const float delta_time )
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
