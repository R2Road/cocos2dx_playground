#include "step_clickclick_game_Stage.h"

#include <cassert>
#include <chrono>
#include <new>
#include <random>

namespace step_clickclick
{
	namespace game
	{
		namespace
		{
			void CheckOddNumber( const int number )
			{
				assert( 1 == ( number & 1 ) );
			}
			void CheckSize( const int pivot, const int number )
			{
				assert( pivot >= number );
			}
			int GetRandomInt( int min, int max )
			{
				static std::random_device rd;
				static std::mt19937 randomEngine( rd() );
				static std::uniform_int_distribution<> dist( min, max );
				return dist( randomEngine );
			}
		}


		Stage::Pannel::Pannel( const int index, const int life ) :
			mIndex( index )
			, mPannelType( ePannelType::Different )
			, mActive( false )
			, mLife( life )
		{}

		void Stage::Pannel::Init( ePannelType type, const int life )
		{
			mPannelType = type;
			mActive = true;
			mLife = life;
		}
		void Stage::Pannel::DecreaseAction()
		{
			mLife = std::max( 0, mLife - 1 );
			mActive = ( 0 < mLife );
		}
		void Stage::Pannel::IncreaseAction()
		{
			mLife = std::min( 100, mLife + 1 );
			mActive = ( 0 < mLife );
		}
		void Stage::Pannel::DieAction()
		{
			mLife = 0;
			mActive = false;
		}



		Stage::Stage( const int width, const int height ) :
			mStageWidth( width )
			, mStageHeight( height )
			, mCenterX( mStageWidth / 2 )
			, mCenterY( mStageWidth / 2 )
			, mGridIndexConverter( mStageWidth, mStageHeight )
			, Pannels()
		{
			//
			// Must odd number
			//
			CheckOddNumber( mStageWidth );
			CheckOddNumber( mStageHeight );
		}

		Stage* Stage::create( const int width, const int height )
		{
			auto ret = new ( std::nothrow ) Stage( width, height );
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			return ret;
		}

		bool Stage::init()
		{
			for( int ty = 0; ty < mStageHeight; ++ty )
			{
				for( int tx = 0; tx < mStageWidth; ++tx )
				{
					const int linear_index = mGridIndexConverter.To_Linear( tx, ty );

					Pannels.emplace_back(
						linear_index
						, 0
					);
				}
			}

			return true;
		}

		void Stage::Setup( const int width, const int height )
		{
			CheckOddNumber( width );
			CheckOddNumber( height );
			CheckSize( mStageWidth, width );
			CheckSize( mStageHeight, height );

			//
			// Clear
			//
			for( auto& p : Pannels )
			{
				p.DieAction();
			}

			std::vector<ePannelType> pannel_type_list;
			{
				const int pannel_count = width * height;
				pannel_type_list.resize( pannel_count, ePannelType::Single );

				// init
				const int together_count = pannel_count * 0.3f;
				const int different_count = pannel_count * 0.2f;
				auto cur = pannel_type_list.begin();
				for( int i = 0; i < together_count; ++i )
				{
					*cur = ePannelType::Together;
					++cur;
				}
				for( int i = 0; i < different_count; ++i )
				{
					*cur = ePannelType::Different;
					++cur;
				}

				// shuffle
				const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine random_engine( seed );
				shuffle( pannel_type_list.begin(), pannel_type_list.end(), random_engine );
				shuffle( pannel_type_list.begin(), pannel_type_list.end(), random_engine );
			}

			const int current_pivot_x = mCenterX - ( width / 2 );
			const int current_pivot_y = mCenterY - ( height / 2 );
			auto t_type = pannel_type_list.cbegin();
			int linear_index = 0;
			for( int ty = current_pivot_y; ty < current_pivot_y + height; ++ty )
			{
				for( int tx = current_pivot_x; tx < current_pivot_x + width; ++tx )
				{
					linear_index = mGridIndexConverter.To_Linear( tx, ty );

					Pannels[linear_index].Init( *t_type, GetRandomInt( 3, 9 ) );
					++t_type;
				}
			}
		}

		const Stage::Pannel& Stage::GetPannelData( const int linear_index ) const
		{
			if( 0 > linear_index || static_cast<int>( Pannels.size() ) <= linear_index )
			{
				static const Pannel dummy( -1, 0 );
				return dummy;
			}

			return Pannels[linear_index];
		}
		void Stage::IncreasePannelLife( const int linear_index )
		{
			if( 0 > linear_index || static_cast<int>( Pannels.size() ) <= linear_index )
			{
				return;
			}

			Pannels[linear_index].IncreaseAction();
		}
		void Stage::DecreasePannelLife( const int linear_index )
		{
			if( 0 > linear_index || static_cast<int>( Pannels.size() ) <= linear_index )
			{
				return;
			}

			Pannels[linear_index].DecreaseAction();
		}
		void Stage::DiePannelLife( const int linear_index )
		{
			if( 0 > linear_index || static_cast<int>( Pannels.size() ) <= linear_index )
			{
				return;
			}

			Pannels[linear_index].DieAction();
		}
	} // namespace game
} // namespace step_clickclick
