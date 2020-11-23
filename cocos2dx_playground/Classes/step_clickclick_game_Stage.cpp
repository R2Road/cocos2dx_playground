#include "step_clickclick_game_Stage.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <new>
#include <random>

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

	int GetRandomInt( int min, int max )
	{
		static std::random_device rd;
		static std::mt19937 randomEngine( rd() );
		static std::uniform_int_distribution<> dist( min, max );
		return dist( randomEngine );
	}
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
			if( !ret || !ret->Init() )
			{
				ret.reset();
				return nullptr;
			}

			return ret;
		}

		bool Stage::Init()
		{
			for( int ty = 0; ty < mStageHeight; ++ty )
			{
				for( int tx = 0; tx < mStageWidth; ++tx )
				{
					const int linear_index = mGridIndexConverter.To_Linear( tx, ty );

					mBlocks.emplace_back( linear_index );
				}
			}

			return true;
		}

		void Stage::Setup( const int width, const int height )
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

			std::vector<eBlockType> block_type_list;
			{
				block_type_list.resize( mActiveBlockCount, eBlockType::Single );

				// init
				const int together_count = mActiveBlockCount * 0.3f;
				const int different_count = mActiveBlockCount * 0.2f;
				auto cur = block_type_list.begin();
				for( int i = 0; i < together_count; ++i )
				{
					*cur = eBlockType::Same;
					++cur;
				}
				for( int i = 0; i < different_count; ++i )
				{
					*cur = eBlockType::Different;
					++cur;
				}

				// shuffle
				const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine random_engine( seed );
				std::shuffle( block_type_list.begin(), block_type_list.end(), random_engine );
				std::shuffle( block_type_list.begin(), block_type_list.end(), random_engine );
			}

			const int current_pivot_x = mCenterX - ( width / 2 );
			const int current_pivot_y = mCenterY - ( height / 2 );
			auto t_type = block_type_list.cbegin();
			int linear_index = 0;
			for( int ty = current_pivot_y; ty < current_pivot_y + height; ++ty )
			{
				for( int tx = current_pivot_x; tx < current_pivot_x + width; ++tx )
				{
					linear_index = mGridIndexConverter.To_Linear( tx, ty );

					mBlocks[linear_index].Reset( *t_type, GetRandomInt( 3, 9 ) );
					++t_type;
				}
			}
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
	} // namespace game
} // namespace step_clickclick
