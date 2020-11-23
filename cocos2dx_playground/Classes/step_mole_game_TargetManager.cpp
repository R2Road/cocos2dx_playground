#include "step_mole_game_TargetManager.h"

#include <new>
#include <numeric> // std::iota
#include <random>

namespace step_mole
{
	namespace game
	{
		TargetManager::TargetManager() :
			mIdleTargetContainer()
			, mIdleTargetIndicator()
			, mRestTargetContainer()
		{}

		TargetManagerUp TargetManager::create( const StageConfig& stage_config )
		{
			TargetManagerUp ret( new ( std::nothrow ) TargetManager() );
			if( !ret )
			{
				ret.reset();
			}
			else
			{
				ret->init( stage_config );
			}

			return ret;
		}

		void TargetManager::init( const StageConfig& stage_config )
		{
			const auto capacity = stage_config.BlockCount_Horizontal* stage_config.BlockCount_Vercital;

			mIdleTargetContainer.reserve( capacity );

			mRestTargetContainer.resize( capacity, -1 );
			std::iota( mRestTargetContainer.begin(), mRestTargetContainer.end(), 0 ); // fill : 0, 1, 2, 3, 4 ......			

			Refill();
		}

		int TargetManager::GetIdleTarget()
		{
			int ret = -1;

			if( mIdleTargetContainer.end() == mIdleTargetIndicator )
			{
				Refill();
				if( mIdleTargetContainer.end() == mIdleTargetIndicator )
				{
					return ret;
				}
			}

			ret = ( *mIdleTargetIndicator );
			++mIdleTargetIndicator;
			return ret;
		}

		void TargetManager::ComeHomeTarget( const int target_index )
		{
			mRestTargetContainer.push_back( target_index );
			CCLOG( "Rest Target Count : %d", mRestTargetContainer.size() );
		}

		void TargetManager::Refill()
		{
			if( mRestTargetContainer.empty() )
			{
				CCLOG( "Refill Impossible" );
				return;
			}

			std::shuffle( mRestTargetContainer.begin(), mRestTargetContainer.end(), std::mt19937{ std::random_device{}( ) } );
			mRestTargetContainer.swap( mIdleTargetContainer );
			mRestTargetContainer.clear();

			mIdleTargetIndicator = mIdleTargetContainer.begin();

			CCLOG( "Refill Successes" );
		}
	}
}
