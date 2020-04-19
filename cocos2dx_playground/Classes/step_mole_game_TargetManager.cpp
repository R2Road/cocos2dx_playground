#include "step_mole_game_TargetManager.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

namespace step_mole
{
	namespace game
	{
		TargetManager::TargetManager() :
			mIdleTarget()
		{}

		TargetManagerUp TargetManager::create( const int width, const int height )
		{
			TargetManagerUp ret( new ( std::nothrow ) TargetManager() );
			if( !ret || !ret->Init( width, height ) )
			{
				ret.reset();
				return nullptr;
			}

			return ret;
		}

		bool TargetManager::Init( const int width, const int height )
		{
			const auto target_count = width * height;
			mIdleTarget.resize( target_count, -1 );
			std::iota( mIdleTarget.begin(), mIdleTarget.end(), 0 ); // fill : 0, 1, 2, 3, 4 ......

			//
			// shuffle for list : from cplusplus.com
			// ...mm not good.
			//
			std::vector<ValueT> temp_vector( mIdleTarget.begin(), mIdleTarget.end() );
			std::shuffle( temp_vector.begin(), temp_vector.end(), std::mt19937{ std::random_device{}() } );
			ContainerT shuffled_list{ temp_vector.begin(), temp_vector.end() };
			mIdleTarget.swap( shuffled_list );

			return true;
		}

		int TargetManager::GetIdleTarget()
		{
			int ret = -1;

			if( mIdleTarget.empty() )
			{
				return ret;
			}

			ret = ( *mIdleTarget.begin() );
			mIdleTarget.pop_front();
			return ret;
		}
	}
}
