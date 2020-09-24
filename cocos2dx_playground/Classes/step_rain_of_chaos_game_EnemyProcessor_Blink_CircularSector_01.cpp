#include "step_rain_of_chaos_game_EnemyProcessor_Blink_CircularSector_01.h"

#include <algorithm>
#include <new>

#include "step_rain_of_chaos_game_EnemyNode.h"
#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Blink_CircularSector_01::EnemyProcessor_Blink_CircularSector_01(
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
			, mbLive( false )
		{}

		EnemyProcessorUp EnemyProcessor_Blink_CircularSector_01::Create(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const float limit_time
			, const bool direction
			, const float degree_per_cycle
		)
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Blink_CircularSector_01( stage_config, owner_node, target_node, limit_time, direction, degree_per_cycle ) );
			ret->init();
			return ret;
		}

		void EnemyProcessor_Blink_CircularSector_01::Enter()
		{
			mMove = mOwnerNode->getPosition() - mStageConfig.GetCenter();
			mMove.rotate( Vec2::ZERO, mRadianPerCycle );

			mElapsedTime = 0.f;

			mbLive = true;

			static_cast<step_rain_of_chaos::game::EnemyNode*>( mOwnerNode )->HideView();
		}

		bool EnemyProcessor_Blink_CircularSector_01::Update( const float delta_time )
		{
			if( !mbLive )
			{
				return false;
			}

			mElapsedTime += delta_time;
			mbLive = mLimitTime > mElapsedTime;

			if( !mbLive )
			{
				mOwnerNode->setPosition( mStageConfig.GetCenter() + mMove );
				static_cast<step_rain_of_chaos::game::EnemyNode*>( mOwnerNode )->ShowView();
			}

			return mbLive;
		}
	}
}
