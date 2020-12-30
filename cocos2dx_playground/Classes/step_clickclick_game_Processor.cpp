#include "step_clickclick_game_Processor.h"

#include <algorithm>
#include <cassert>

#include "audio/include/AudioEngine.h"

#include "step_clickclick_game_EffectManagerNode.h"
#include "step_clickclick_game_Stage.h"
#include "step_clickclick_game_StageView.h"

namespace
{
	void ProcessEffect( step_clickclick::game::EffectManagerNode* effect_manager_node, const int block_index, const int last_life, const int current_life )
	{
		if( 0 == current_life )
		{
			effect_manager_node->PlayEffect( block_index, step_clickclick::game::eEffectIndex::Die );
		}
		else if( last_life < current_life )
		{
			effect_manager_node->PlayEffect( block_index, step_clickclick::game::eEffectIndex::Increase );
		}
		else
		{
			effect_manager_node->PlayEffect( block_index, step_clickclick::game::eEffectIndex::Decrease );
		}
	}
}

namespace step_clickclick
{
	namespace game
	{
		void Processor::Do( Stage* stage, StageView* stage_view, EffectManagerNode* effect_manager_node, const int block_linear_index, int* out_score )
		{
			const auto& pivot_block_data = stage->GetBlockData( block_linear_index );
			const auto pivot_block_point_index = stage->ConvertLinearIndex2PointIndex( pivot_block_data.GetIndex() );
			int last_life = 0;

			if( eBlockType::Single == pivot_block_data.GetType() )
			{
				cocos2d::experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.1f );

				const int start_x = pivot_block_point_index.x - 1;
				const int start_y = pivot_block_point_index.y - 1;
				const int end_x = start_x + 3;
				const int end_y = start_y + 3;

				bool has_neighbor = false;
				for( int cur_y = start_y; cur_y < end_y; ++cur_y )
				{
					for( int cur_x = start_x; cur_x < end_x; ++cur_x )
					{
						if( !stage->isIn( cur_x, cur_y ) )
						{
							continue;
						}

						// check up, down, left, right
						if( cur_x != pivot_block_point_index.x && cur_y != pivot_block_point_index.y )
						{
							continue;
						}

						const auto& target_block_data = stage->GetBlockData( cur_x, cur_y );

						// is pivot
						if( pivot_block_data.GetIndex() == target_block_data.GetIndex() )
						{
							continue;
						}

						if( !target_block_data.IsActive() )
						{
							continue;
						}

						has_neighbor = true;
						break;
					}
				}

				last_life = pivot_block_data.GetLife();
				if( has_neighbor )
				{
					++*out_score;

					stage->DecreaseBlockLife( pivot_block_data.GetIndex() );
					stage_view->UpdateBlock( pivot_block_data.GetIndex(), last_life, pivot_block_data.GetLife() );
				}
				else
				{
					*out_score = std::max( 0, *out_score - pivot_block_data.GetLife() );

					stage->DieBlock( pivot_block_data.GetIndex() );
					stage_view->UpdateBlock( pivot_block_data.GetIndex(), last_life, pivot_block_data.GetLife() );
				}

				ProcessEffect( effect_manager_node, pivot_block_data.GetIndex(), last_life, pivot_block_data.GetLife() );
			}
			else if( eBlockType::Same == pivot_block_data.GetType() )
			{
				cocos2d::experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );

				const int pivot_life = pivot_block_data.GetLife();

				const int start_x = pivot_block_point_index.x - 1;
				const int start_y = pivot_block_point_index.y - 1;
				const int end_x = start_x + 3;
				const int end_y = start_y + 3;

				for( int cur_y = start_y; cur_y < end_y; ++cur_y )
				{
					for( int cur_x = start_x; cur_x < end_x; ++cur_x )
					{
						if( !stage->isIn( cur_x, cur_y ) )
						{
							continue;
						}

						const auto& target_block_data = stage->GetBlockData( cur_x, cur_y );
						if( !target_block_data.IsActive() )
						{
							continue;
						}

						if( eBlockType::Same == target_block_data.GetType() && pivot_life != target_block_data.GetLife() )
						{
							continue;
						}

						last_life = target_block_data.GetLife();
						if( pivot_life != target_block_data.GetLife() )
						{
							stage->IncreaseBlockLife( target_block_data.GetIndex() );
						}
						else
						{
							*out_score += 3;
							stage->DecreaseBlockLife( target_block_data.GetIndex() );
						}

						stage_view->UpdateBlock( target_block_data.GetIndex(), last_life, target_block_data.GetLife() );
						ProcessEffect( effect_manager_node, target_block_data.GetIndex(), last_life, target_block_data.GetLife() );
					}
				}
			}
			else if( eBlockType::Different == pivot_block_data.GetType() )
			{
				cocos2d::experimental::AudioEngine::play2d( "sounds/fx/coin_001.ogg", false, 0.2f );

				const int pivot_life = pivot_block_data.GetLife();

				const int start_x = pivot_block_point_index.x - 1;
				const int start_y = pivot_block_point_index.y - 1;
				const int end_x = start_x + 3;
				const int end_y = start_y + 3;

				for( int cur_y = start_y; cur_y < end_y; ++cur_y )
				{
					for( int cur_x = start_x; cur_x < end_x; ++cur_x )
					{
						if( !stage->isIn( cur_x, cur_y ) )
						{
							continue;
						}

						const auto& target_block_data = stage->GetBlockData( cur_x, cur_y );
						if( !target_block_data.IsActive() )
						{
							continue;
						}

						last_life = target_block_data.GetLife();
						if( target_block_data.GetIndex() != pivot_block_data.GetIndex() && pivot_life == target_block_data.GetLife() )
						{
							stage->IncreaseBlockLife( target_block_data.GetIndex() );
							stage->IncreaseBlockLife( target_block_data.GetIndex() );
							stage->IncreaseBlockLife( target_block_data.GetIndex() );
							stage->IncreaseBlockLife( target_block_data.GetIndex() );
						}
						else
						{
							*out_score += target_block_data.GetLife();
							stage->DieBlock( target_block_data.GetIndex() );
						}

						stage_view->UpdateBlock( target_block_data.GetIndex(), last_life, target_block_data.GetLife() );
						ProcessEffect( effect_manager_node, target_block_data.GetIndex(), last_life, target_block_data.GetLife() );
					}
				}
			}
		}
	}
}
