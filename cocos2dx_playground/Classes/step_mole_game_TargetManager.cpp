#include "step_mole_game_TargetManager.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#include "base/CCConsole.h"
#include "platform/CCPlatformMacros.h"

namespace step_mole
{
	namespace game
	{
		TargetManager::TargetManager() :
			mIdleTarget()
		{}

		TargetManagerUp TargetManager::create( const StageConfig& stage_config )
		{
			TargetManagerUp ret( new ( std::nothrow ) TargetManager() );
			if( !ret || !ret->Init( stage_config ) )
			{
				ret.reset();
				return nullptr;
			}

			return ret;
		}

		bool TargetManager::Init( const StageConfig& stage_config )
		{
			const auto target_count = stage_config.BlockCount_Horizontal* stage_config.BlockCount_Vercital;
			mIdleTarget.resize( target_count, -1 );
			std::iota( mIdleTarget.begin(), mIdleTarget.end(), 0 ); // fill : 0, 1, 2, 3, 4 ......

			
			Shuffle( mIdleTarget );

			return true;
		}

		void TargetManager::Shuffle( ContainerT& target_container )
		{
			//
			// shuffle for list : from cplusplus.com
			// ...mm not good.
			//
			std::vector<ValueT> temp_vector( target_container.begin(), target_container.end() );
			std::shuffle( temp_vector.begin(), temp_vector.end(), std::mt19937{ std::random_device{}( ) } );
			ContainerT shuffled_list{ temp_vector.begin(), temp_vector.end() };
			target_container.swap( shuffled_list );
		}

		int TargetManager::GetIdleTarget()
		{
			int ret = -1;

			if( mIdleTarget.empty() )
			{
				Refill();
				if( mIdleTarget.empty() )
				{
					return ret;
				}
			}

			ret = ( *mIdleTarget.begin() );
			mIdleTarget.pop_front();
			return ret;
		}

		void TargetManager::ComeHomeTarget( const int target_index )
		{
			mRestTarget.push_front( target_index );
			CCLOG( "Rest Target Count : %d", mRestTarget.size() );
		}

		void TargetManager::Refill()
		{
			if( mRestTarget.empty() )
			{
				CCLOG( "Refill Impossible" );
				return;
			}

			Shuffle( mRestTarget );
			mIdleTarget.splice( mIdleTarget.end(), mRestTarget );
			CCLOG( "Refill Successes" );
		}
	}
}
