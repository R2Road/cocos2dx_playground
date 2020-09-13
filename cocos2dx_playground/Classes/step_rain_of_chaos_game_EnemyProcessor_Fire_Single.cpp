#include "step_rain_of_chaos_game_EnemyProcessor_Fire_Single.h"

#include <algorithm>
#include <new>

#include "2d/CCNode.h"

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Fire_Single::EnemyProcessor_Fire_Single(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, SpawnProcessorUp&& spawn_processor
			, SpawnInfoContainer& spawn_info_container
		) : iEnemyProcessor( stage_config, owner_node, target_node )
			, mSpawnProcessor( std::move( spawn_processor ) )
			, mSpawnInfoContainer( spawn_info_container )
			, mIsLive( true )
		{}

		EnemyProcessorUp EnemyProcessor_Fire_Single::Create(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, SpawnProcessorUp&& spawn_processor
			, SpawnInfoContainer& spawn_info_container
		)
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Fire_Single(
				stage_config
				, owner_node
				, target_node
				, std::move( spawn_processor )
				, spawn_info_container )
			);
			ret->init();
			return ret;
		}

		void EnemyProcessor_Fire_Single::Enter()
		{
			mSpawnProcessor->Enter( mOwnerNode->getPosition(), mTargetNode->getPosition() );
			mIsLive = true;
		}

		bool EnemyProcessor_Fire_Single::Update( const float delta_time )
		{
			if( mIsLive )
			{
				mIsLive = mSpawnProcessor->Update( delta_time, mOwnerNode->getPosition(), mTargetNode->getPosition(), &mSpawnInfoContainer );
			}

			return mIsLive;
		}
	}
}
