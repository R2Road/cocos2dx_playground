#include "step_rain_of_chaos_game_EnemyProcessor_Move_CircularSector_2Target_01.h"

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
		EnemyProcessor_Move_CircularSector_2Target_01::EnemyProcessor_Move_CircularSector_2Target_01(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
		) : iEnemyProcessor( stage_config, owner_node, target_node )
			, mLimitTime( limit_time )

			, mRadianPerCycle( 0.f )
			, mPivot()

			, mElapsedTime( 0.f )
		{}

		EnemyProcessorUp EnemyProcessor_Move_CircularSector_2Target_01::Create(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
		)
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Move_CircularSector_2Target_01( stage_config, owner_node, target_node, limit_time ) );
			ret->init();
			return ret;
		}

		void EnemyProcessor_Move_CircularSector_2Target_01::Enter()
		{
			mPivot = mOwnerNode->getPosition() - mStageConfig.GetCenter();

			if( MATH_EPSILON > mTargetNode->getPosition().distance( mStageConfig.GetCenter() ) )
			{
				mRadianPerCycle = CC_DEGREES_TO_RADIANS( 90.f * ( cpg::Random::GetBool() ? -1.f : 1.f ) );
			}
			else
			{
				Vec2 Center2Player = mTargetNode->getPosition() - mStageConfig.GetCenter();
				Center2Player.normalize();
				Vec2 Center2Enemy = mOwnerNode->getPosition() - mStageConfig.GetCenter();
				Center2Enemy.normalize();

				const auto dot_result = Center2Enemy.dot( Center2Player );
				const auto theta = acos( dot_result );
				if( 0.f <= Center2Enemy.cross( Center2Player ) )
				{
					mRadianPerCycle = theta * 1.f;
				}
				else
				{
					mRadianPerCycle = theta * -1.f;
				}
			}

			mElapsedTime = 0.f;
		}

		bool EnemyProcessor_Move_CircularSector_2Target_01::Update( const float delta_time )
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
