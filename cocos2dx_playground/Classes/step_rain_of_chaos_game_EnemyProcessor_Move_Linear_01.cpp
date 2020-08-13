#include "step_rain_of_chaos_game_EnemyProcessor_Move_Linear_01.h"

#include <algorithm>
#include <new>

#include "2d/CCNode.h"

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Move_Linear_01::EnemyProcessor_Move_Linear_01( const StageConfig& stage_config, Node* const owner_node ) : iEnemyProcessor( stage_config, owner_node )
			, mStartPosition()
			, mMove()

			, mLimitTime( 0.3f )
			, mElapsedTime( 0.f )
		{}

		EnemyProcessorUp EnemyProcessor_Move_Linear_01::Create( const StageConfig& stage_config, Node* const owner_node )
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Move_Linear_01( stage_config, owner_node ) );
			ret->init();
			return ret;
		}

		void EnemyProcessor_Move_Linear_01::Enter()
		{
			bool direction = true;
			float radian = CC_DEGREES_TO_RADIANS( 180.f ) * ( direction ? 1 : -1 );

			mStartPosition = mOwnerNode->getPosition();

			auto center_to_start = mStartPosition - mStageConfig.GetCenter();
			auto center_to_end = center_to_start;
			center_to_end.rotate( Vec2::ZERO, radian );

			mMove = center_to_end - center_to_start;

			mElapsedTime = 0.f;
		}

		bool EnemyProcessor_Move_Linear_01::Update( const float delta_time )
		{
			mElapsedTime += delta_time;

			float rate = mElapsedTime / mLimitTime;
			rate = std::min( 1.f, rate );

			mOwnerNode->setPosition( mStartPosition + ( mMove * rate ) );

			return mLimitTime > mElapsedTime;
		}
	}
}
