#include "step_clickclick_game_Processor.h"

#include <algorithm>
#include <cassert>

#include "audio/include/AudioEngine.h"

#include "step_clickclick_game_Stage.h"
#include "step_clickclick_game_StageView.h"

namespace step_clickclick
{
	namespace game
	{
		void Processor::Do( Stage* stage, StageView* stage_view, const int block_linear_index, int* out_score )
		{
			const auto& block_data = stage->GetBlockData( block_linear_index );
			const auto block_point_index = stage->ConvertLinearIndex2PointIndex( block_data.GetIndex() );
			int last_life = 0;

			if( eBlockType::Single == block_data.GetType() )
			{
				cocos2d::experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.1f );

				const int start_x = block_point_index.x - 1;
				const int start_y = block_point_index.y - 1;
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
						if( cur_x != block_point_index.x && cur_y != block_point_index.y )
						{
							continue;
						}

						const auto& target_block_data = stage->GetBlockData( cur_x, cur_y );

						// is pivot
						if( block_linear_index == target_block_data.GetIndex() )
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

				last_life = block_data.GetLife();
				if( has_neighbor )
				{
					++*out_score;

					stage->DecreaseBlockLife( block_data.GetIndex() );
					stage_view->UpdateBlock( block_data.GetIndex(), last_life, block_data.GetLife() );
				}
				else
				{
					*out_score = std::max( 0, *out_score - block_data.GetLife() );

					stage->DieBlock( block_data.GetIndex() );
					stage_view->UpdateBlock( block_data.GetIndex(), last_life, block_data.GetLife() );
				}
			}
			else if( eBlockType::Same == block_data.GetType() )
			{
				cocos2d::experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );

				const int pivot_count = block_data.GetLife();

				const int start_x = block_point_index.x - 1;
				const int start_y = block_point_index.y - 1;
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

						if( eBlockType::Same == target_block_data.GetType() && pivot_count != target_block_data.GetLife() )
						{
							continue;
						}

						last_life = target_block_data.GetLife();
						if( pivot_count != target_block_data.GetLife() )
						{
							stage->IncreaseBlockLife( target_block_data.GetIndex() );
						}
						else
						{
							*out_score += 3;
							stage->DecreaseBlockLife( target_block_data.GetIndex() );
						}

						stage_view->UpdateBlock( target_block_data.GetIndex(), last_life, target_block_data.GetLife() );
					}
				}
			}
			else if( eBlockType::Different == block_data.GetType() )
			{
				cocos2d::experimental::AudioEngine::play2d( "sounds/fx/coin_001.ogg", false, 0.2f );

				const int pivot_life = block_data.GetLife();

				const int start_x = block_point_index.x - 1;
				const int start_y = block_point_index.y - 1;
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
						if( target_block_data.GetIndex() != block_data.GetIndex() && pivot_life == target_block_data.GetLife() )
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
					}
				}
			}
		}
	}
}
