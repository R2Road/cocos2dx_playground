#include "step_rain_of_chaos_game_EnemyProcessor_Move_Linear_2Target_01.h"

#include <algorithm>
#include <new>

#include "2d/CCNode.h"

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Move_Linear_2Target_01::EnemyProcessor_Move_Linear_2Target_01(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
		) : iEnemyProcessor( stage_config, owner_node, target_node )
			, mLimitTime( limit_time )

			, mStartPosition()
			, mMove()

			, mElapsedTime( 0.f )
		{}

		EnemyProcessorUp EnemyProcessor_Move_Linear_2Target_01::Create(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
		)
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Move_Linear_2Target_01( stage_config, owner_node, target_node, limit_time ) );
			ret->init();
			return ret;
		}

		void EnemyProcessor_Move_Linear_2Target_01::Enter()
		{
			mStartPosition = mOwnerNode->getPosition();

			if( MATH_EPSILON > mTargetNode->getPosition().distance( mOwnerNode->getPosition() ) )
			{
				//
				// Do Pass
				//
				mMove = Vec2::ZERO;
				mElapsedTime = mLimitTime;
			}
			else
			{
				Vec2 Enemy2Player = mTargetNode->getPosition() - mStartPosition;
				Enemy2Player.normalize();
				Vec2 Enemy2Center = mStageConfig.GetCenter() - mStartPosition;
				Enemy2Center.normalize();

				//
				// # dot
				// ( dot_result = 0 ) == 90`
				// ( dot_result < 0 ) > 90`
				// ( dot_result > 0 ) < 90`
				//
				const auto dot_result = Enemy2Player.dot( Enemy2Center );

				//
				// # warning
				// "acos" some time return "nan"
				// dot_result >= 1
				//
				const auto theta = dot_result >= 1.0f ? 0.f : acos( dot_result );
				const auto degree = CC_RADIANS_TO_DEGREES( theta );
				
				//CCLOG( "E2P : %f, %f", Enemy2Player.x, Enemy2Player.y );
				//CCLOG( "E2C : %f, %f", Enemy2Center.x, Enemy2Center.y );
				//CCLOG( "dot : %f", dot_result );
				//CCLOG( "theta : %f", theta );
				//CCLOG( "degree : %f", degree );

				//
				// # make rotation angle
				// - rotation angle : angle for Center2Enemy to Center2TargetPosition
				// - total inner angle for triangle : 180
				// - enemy position is on the circle
				// - target position is on the circle
				// - two coner angle is same
				// - degree + degree + x = 180
				//
				const auto degree4E2T = 180.f - ( degree * 2 );

				Vec2 Center2Enemy = mStartPosition - mStageConfig.GetCenter();
				if( 0.f <= Enemy2Center.cross( Enemy2Player ) ) // check rotation direction
				{
					Center2Enemy.rotate( Vec2::ZERO, -CC_DEGREES_TO_RADIANS( degree4E2T ) );
				}
				else
				{
					Center2Enemy.rotate( Vec2::ZERO, CC_DEGREES_TO_RADIANS( degree4E2T ) );
				}

				const Vec2 target_position = mStageConfig.GetCenter() + Center2Enemy;
				mMove = target_position - mStartPosition;
			}

			mElapsedTime = 0.f;
		}

		bool EnemyProcessor_Move_Linear_2Target_01::Update( const float delta_time )
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
