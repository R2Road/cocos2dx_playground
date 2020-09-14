#include "step_rain_of_chaos_game_EnemyProcessor_Fire_Chain.h"

#include <algorithm>
#include <new>

#include "2d/CCNode.h"

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Fire_Chain::EnemyProcessor_Fire_Chain(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, SpawnProcessorPackage&& spawn_processor_container
			, SpawnInfoContainer& spawn_info_container
		) : iEnemyProcessor( stage_config, owner_node, target_node )
			, mSpawnProcessorContainer( std::move( spawn_processor_container ) )
			, mCurrentSpawnProcessor()
			, mSpawnInfoContainer( spawn_info_container )
		{}

		EnemyProcessorUp EnemyProcessor_Fire_Chain::Create(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, SpawnProcessorPackage&& spawn_processor_container
			, SpawnInfoContainer& spawn_info_container
		)
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Fire_Chain(
				stage_config
				, owner_node
				, target_node
				, std::move( spawn_processor_container )
				, spawn_info_container )
			);
			ret->init();
			return ret;
		}

		void EnemyProcessor_Fire_Chain::Enter()
		{
			mCurrentSpawnProcessor = mSpawnProcessorContainer.begin();
			( *mCurrentSpawnProcessor )->Enter( mOwnerNode->getPosition(), mTargetNode->getPosition() );
		}

		bool EnemyProcessor_Fire_Chain::Update( const float delta_time )
		{
			if( mSpawnProcessorContainer.end() == mCurrentSpawnProcessor )
			{
				return false;
			}

			if( !( *mCurrentSpawnProcessor )->Update( delta_time, mOwnerNode->getPosition(), mTargetNode->getPosition(), &mSpawnInfoContainer ) )
			{
				++mCurrentSpawnProcessor;
				if( mSpawnProcessorContainer.end() != mCurrentSpawnProcessor )
				{
					( *mCurrentSpawnProcessor )->Enter( mOwnerNode->getPosition(), mTargetNode->getPosition() );
				}
			}

			return true;
		}
	}
}
