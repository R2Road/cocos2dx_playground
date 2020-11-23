#include "step_mole_game_TargetManager.h"

#include <algorithm>
#include <numeric>
#include <random>

#include "base/CCConsole.h"
#include "platform/CCPlatformMacros.h"

namespace step_mole
{
	namespace game
	{
		TargetManager::TargetManager() :
			mIdleTarget()
			, mIdleTargetIndicator()
			, mRestTarget()
		{}

		TargetManagerUp TargetManager::create( const StageConfig& stage_config )
		{
			TargetManagerUp ret( new ( std::nothrow ) TargetManager() );
			if( !ret || !ret->init( stage_config ) )
			{
				ret.reset();
				return nullptr;
			}

			return ret;
		}

		bool TargetManager::init( const StageConfig& stage_config )
		{
			const auto capacity = stage_config.BlockCount_Horizontal* stage_config.BlockCount_Vercital;

			mIdleTarget.resize( capacity, -1 );
			std::iota( mIdleTarget.begin(), mIdleTarget.end(), 0 ); // fill : 0, 1, 2, 3, 4 ......			
			shuffle( mIdleTarget );
			mIdleTargetIndicator = mIdleTarget.begin();

			mRestTarget.reserve( capacity );

			return true;
		}

		void TargetManager::shuffle( ContainerT& target_container )
		{
			std::shuffle( target_container.begin(), target_container.end(), std::mt19937{ std::random_device{}( ) } );
		}

		int TargetManager::GetIdleTarget()
		{
			int ret = -1;

			if( mIdleTarget.end() == mIdleTargetIndicator )
			{
				Refill();
				if( mIdleTarget.end() == mIdleTargetIndicator )
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
			mRestTarget.push_back( target_index );
			CCLOG( "Rest Target Count : %d", mRestTarget.size() );
		}

		void TargetManager::Refill()
		{
			if( mRestTarget.empty() )
			{
				CCLOG( "Refill Impossible" );
				return;
			}

			shuffle( mRestTarget );
			mRestTarget.swap( mIdleTarget );
			mRestTarget.clear();

			mIdleTargetIndicator = mIdleTarget.begin();

			CCLOG( "Refill Successes" );
		}
	}
}
