#include "step_rain_of_chaos_game_EnemyProcessor_Tie.h"

#include <new>
#include <utility>

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Tie::EnemyProcessor_Tie(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, EnemyProcessorUp&& processor_1
			, EnemyProcessorUp&& processor_2
		) : iEnemyProcessor( stage_config, owner_node, target_node )
			, mProcessor_1( std::move( processor_1 ) )
			, mProcessor_2( std::move( processor_2 ) )
		{}

		EnemyProcessorUp EnemyProcessor_Tie::Create(
			const StageConfig& stage_config
			, Node* const owner_node
			, Node* const target_node
			, EnemyProcessorUp&& processor_1
			, EnemyProcessorUp&& processor_2
		)
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Tie(
				stage_config
				, owner_node
				, target_node
				, std::move( processor_1 )
				, std::move( processor_2 )
			) );
			ret->init();
			return ret;
		}

		void EnemyProcessor_Tie::Enter()
		{
			mProcessor_1->Enter();
			mProcessor_2->Enter();
		}

		bool EnemyProcessor_Tie::Update( const float delta_time )
		{
			const auto result_1 = mProcessor_1->Update( delta_time );
			const auto result_2 = mProcessor_2->Update( delta_time );

			return result_1 || result_2;
		}
	}
}
