#include "step_clickclick_game_Stage.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <new>
#include <random>

#include "cpg_Random.h"

namespace
{

#if defined( DEBUG ) || defined( _DEBUG )
	#define CHECK_ODD_NUMBER( number ) ( assert( 1 == ( ( number ) & 1 ) ) )
	#define CHECK_SIZE( pivot, number ) ( assert( pivot >= number ) )
	#define CHECK_LINEAR_INDEX( min_index, max_index, defendant ) ( assert( min_index <= defendant && max_index > defendant ) )
#else
	#define CHECK_ODD_NUMBER( number )
	#define CHECK_SIZE( pivot, number )
	#define CHECK_LINEAR_INDEX( min_index, max_index, defendant )
#endif
}

namespace step_clickclick
{
	namespace game
	{
		Stage::Stage( const int width, const int height ) :
			mStageWidth( width )
			, mStageHeight( height )
			, mCenterX( mStageWidth / 2 )
			, mCenterY( mStageWidth / 2 )
			, mGridIndexConverter( mStageWidth, mStageHeight )
			, mBlocks()
			, mActiveBlockCount( 0 )
		{
			//
			// Must odd number
			//
			CHECK_ODD_NUMBER( mStageWidth );
			CHECK_ODD_NUMBER( mStageHeight );
		}

		StageUp Stage::create( const int width, const int height )
		{
			StageUp ret( new ( std::nothrow ) Stage( width, height ) );
			if( !ret )
			{
				ret.reset();
			}
			else
			{
				ret->init();
			}

			return ret;
		}

		void Stage::init()
		{
			for( int ty = 0; ty < mStageHeight; ++ty )
			{
				for( int tx = 0; tx < mStageWidth; ++tx )
				{
					const int linear_index = mGridIndexConverter.To_Linear( tx, ty );

					mBlocks.emplace_back( linear_index );
				}
			}
		}

		void Stage::Setup( const int width, const int height, const int shuffle_limit )
		{
			CHECK_ODD_NUMBER( width );
			CHECK_ODD_NUMBER( height );
			CHECK_SIZE( mStageWidth, width );
			CHECK_SIZE( mStageHeight, height );

			mActiveBlockCount = width * height;

			//
			// Clear
			//
			for( auto& p : mBlocks )
			{
				p.Die();
			}

			//
			// Build Block Type list
			//
			std::vector<eBlockType> block_type_list;
			{
				// Fill Block : Single
				block_type_list.resize( mActiveBlockCount, eBlockType::Single );

				auto cur = block_type_list.begin();

				// Fill Block : Same
				const int same_block_count = mActiveBlockCount * 0.3f;
				for( int i = 0; i < same_block_count; ++i )
				{
					*cur = eBlockType::Same;
					++cur;
				}

				// Fill Block : Different
				const int different_block_count = mActiveBlockCount * 0.2f;
				for( int i = 0; i < different_block_count; ++i )
				{
					*cur = eBlockType::Different;
					++cur;
				}

				// Shuffle x 2
				const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine random_engine( seed );
				for( int i = 0; shuffle_limit > i; ++i )
				{
					std::shuffle( block_type_list.begin(), block_type_list.end(), random_engine );
				}
			}

			//
			// Block Setup
			//
			{
				const int start_x = mCenterX - ( width / 2 );
				const int start_y = mCenterY - ( height / 2 );
				const int end_x = start_x + width;
				const int end_y = start_y + height;

				auto itr_block_type = block_type_list.cbegin();
				int linear_index = 0;
				for( int cur_y = start_y; cur_y < end_y; ++cur_y )
				{
					for( int cur_x = start_x; cur_x < end_x; ++cur_x )
					{
						linear_index = mGridIndexConverter.To_Linear( cur_x, cur_y );

						mBlocks[linear_index].Reset( *itr_block_type, cpg::Random::GetInt( 3, 9 ) );
						++itr_block_type;
					}
				}
			}
		}

		bool Stage::isIn( const int x, const int y ) const
		{
			if( 0 > y || mStageWidth <= y )
			{
				return false;
			}

			if( 0 > x || mStageHeight <= x )
			{
				return false;
			}

			return true;
		}

		const Block& Stage::GetBlockData( const int x, const int y ) const
		{
			return GetBlockData( mGridIndexConverter.To_Linear( x, y )	 );
		}
		const Block& Stage::GetBlockData( const int linear_index ) const
		{
			if( 0 > linear_index || static_cast<int>( mBlocks.size() ) <= linear_index )
			{
				static const Block dummy( -1 );
				return dummy;
			}

			return mBlocks[linear_index];
		}
		void Stage::IncreaseBlockLife( const int linear_index )
		{
			CHECK_LINEAR_INDEX( 0, static_cast<int>( mBlocks.size() ), linear_index );

			mBlocks[linear_index].IncreaseLife();
		}
		void Stage::DecreaseBlockLife( const int linear_index )
		{
			CHECK_LINEAR_INDEX( 0, static_cast<int>( mBlocks.size() ), linear_index );
			assert( mBlocks[linear_index].IsActive() );

			mBlocks[linear_index].DecreaseLife();
			if( 0 == mBlocks[linear_index].GetLife() )
			{
				--mActiveBlockCount;
			}
		}
		void Stage::DieBlock( const int linear_index )
		{
			CHECK_LINEAR_INDEX( 0, static_cast<int>( mBlocks.size() ), linear_index );
			assert( mBlocks[linear_index].IsActive() );

			mBlocks[linear_index].Die();
			--mActiveBlockCount;
		}
	}
}
