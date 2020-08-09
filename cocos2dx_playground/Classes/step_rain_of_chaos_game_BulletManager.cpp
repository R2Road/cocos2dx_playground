#include "step_rain_of_chaos_game_BulletManager.h"

#include <algorithm>
#include <numeric> // std::iota

#include "base/CCConsole.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		BulletManager::BulletManager() :
			mBulletAmount( 0 )
			, mIdleTarget()
			, mRestTarget()
			, mLiveTargetContainer()
		{}
		BulletManager::~BulletManager()
		{
			CCLOG( "BulletManager : BulletAmount : %d", mBulletAmount );
		}

		BulletManagerUp BulletManager::create()
		{
			BulletManagerUp ret( new ( std::nothrow ) BulletManager() );
			if( !ret )
			{
				ret.reset();
				return nullptr;
			}

			return ret;
		}

		BulletManager::ComeHomeCallback BulletManager::GetComeHomeCallback()
		{
			return std::bind( &BulletManager::ComeHomeTarget, this, std::placeholders::_1 );
		}

		void BulletManager::RequestGenerate( const int amount )
		{
			const auto increase_amount = std::max( 1, amount );

			const auto start_index = mBulletAmount;
			const auto end_index = mBulletAmount + increase_amount;

			mBulletAmount += increase_amount;
			mIdleTarget.reserve( mBulletAmount );
			mRestTarget.reserve( mBulletAmount );

			for( int i = start_index; end_index > i; ++i )
			{
				mIdleTarget.emplace_back( i );
			}
		}
		int BulletManager::GetIdleTarget()
		{
			int ret = -1;

			if( mIdleTarget.empty() )
			{
				if( !Refill() )
				{
					return ret;
				}
			}

			ret = ( *mIdleTarget.rbegin() );
			mIdleTarget.pop_back();

			mLiveTargetContainer.push_back( ret );

			return ret;
		}

		void BulletManager::ComeHomeTarget( const int target_index )
		{
			mLiveTargetContainer.remove( target_index );
			mRestTarget.push_back( target_index );
			CCLOG( "Rest Target Count : %d", mRestTarget.size() );
		}

		bool BulletManager::Refill()
		{
			if( mRestTarget.empty() )
			{
				CCLOG( "Refill Impossible" );
				return false;
			}

			std::swap( mIdleTarget, mRestTarget );
			CCLOG( "Refill Successes" );

			return true;
		}
	}
}
