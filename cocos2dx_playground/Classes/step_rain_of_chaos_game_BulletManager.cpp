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
		{}
		BulletManager::~BulletManager()
		{
			CCLOG( "BulletManager : BulletAmount : %d", mBulletAmount );
		}

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
			RequestGenerate( caching_amount );

			return true;
		}

		void BulletManager::RequestGenerate( const int amount )
		{
			const auto target_count = std::max( 1, amount );

			ContainerT temp_container;
			temp_container.resize( target_count, -1 );
			std::iota( temp_container.begin(), temp_container.end(), mBulletAmount ); // fill : 0, 1, 2, 3, 4 ......
			mIdleTarget.splice( mIdleTarget.end(), temp_container );

			mBulletAmount += amount;
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
