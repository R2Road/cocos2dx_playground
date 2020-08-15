#include "step_rain_of_chaos_game_EnemyProcessor_Fire.h"

#include <algorithm>
#include <new>

#include "2d/CCNode.h"

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Fire::EnemyProcessor_Fire(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const RequestBulletCallback& request_bullet_callback
			, SpawnProcessorContainer&& spawn_processor_container
		) : iEnemyProcessor( stage_config, owner_node, target_node )
			, mRequestBulletCallback( request_bullet_callback )
			, mSpawnProcessorContainer( std::move( spawn_processor_container ) )
			, mCurrentSpawnProcessor()
			, mSpawnInfoContainer()
		{
			mSpawnInfoContainer.reserve( 100 );
		}

		EnemyProcessorUp EnemyProcessor_Fire::Create(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, const RequestBulletCallback& request_bullet_callback
			, SpawnProcessorContainer&& spawn_processor_container
		)
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Fire( stage_config, owner_node, target_node, request_bullet_callback, std::move( spawn_processor_container ) ) );
			ret->init();
			return ret;
		}

		void EnemyProcessor_Fire::Enter()
		{
			mCurrentSpawnProcessor = mSpawnProcessorContainer.begin();
			( *mCurrentSpawnProcessor )->Enter( mOwnerNode->getPosition(), mTargetNode->getPosition() );
		}

		bool EnemyProcessor_Fire::Update( const float delta_time )
		{
			if( mSpawnProcessorContainer.end() == mCurrentSpawnProcessor )
			{
				return false;
			}

			mSpawnInfoContainer.clear();

			if( !( *mCurrentSpawnProcessor )->Update( delta_time, mOwnerNode->getPosition(), mTargetNode->getPosition(), &mSpawnInfoContainer ) )
			{
				++mCurrentSpawnProcessor;
				if( mSpawnProcessorContainer.end() != mCurrentSpawnProcessor )
				{
					( *mCurrentSpawnProcessor )->Enter( mOwnerNode->getPosition(), mTargetNode->getPosition() );
				}
			}

			if( !mSpawnInfoContainer.empty() )
			{
				for( const auto& s : mSpawnInfoContainer )
				{
					Vec2 dir = s.MoveDirection;
					dir.normalize();
					dir.scale( 3.f );
					mRequestBulletCallback( s.StartPosition, dir );
				}
			}
		}
	}
}
