#include "step_rain_of_chaos_game_BulletManager.h"

#include <algorithm>
#include <numeric> // std::iota

#include "base/CCConsole.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		BulletManager::BulletManager() :
			mIdleTarget()
		{}

		BulletManagerUp BulletManager::create( const int caching_amount )
		{
			BulletManagerUp ret( new ( std::nothrow ) BulletManager() );
			if( !ret || !ret->Init( caching_amount ) )
			{
				ret.reset();
				return nullptr;
			}

			return ret;
		}

		bool BulletManager::Init( const int caching_amount )
		{
			const auto target_count = std::max( 1, caching_amount );
			mIdleTarget.resize( target_count, -1 );
			std::iota( mIdleTarget.begin(), mIdleTarget.end(), 0 ); // fill : 0, 1, 2, 3, 4 ......

			return true;
		}

		BulletManager::ComeHomeCallback BulletManager::GetComeHomeCallback()
		{
			return std::bind( &BulletManager::ComeHomeTarget, this, std::placeholders::_1 );
		}
		int BulletManager::GetIdleTarget()
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

		void BulletManager::ComeHomeTarget( const int target_index )
		{
			mRestTarget.push_front( target_index );
			CCLOG( "Rest Target Count : %d", mRestTarget.size() );
		}

		void BulletManager::Refill()
		{
			if( mRestTarget.empty() )
			{
				CCLOG( "Refill Impossible" );
				return;
			}

			mIdleTarget.splice( mIdleTarget.end(), mRestTarget );
			CCLOG( "Refill Successes" );
		}
	}
}
