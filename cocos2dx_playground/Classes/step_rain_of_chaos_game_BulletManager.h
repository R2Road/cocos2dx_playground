#pragma once

#include <functional>
#include <list>
#include <memory>

namespace step_rain_of_chaos
{
	namespace game
	{
		using BulletManagerUp = std::unique_ptr<class BulletManager>;

		class BulletManager
		{
		private:
			using ValueT = int;
			using ContainerT = std::list<ValueT>;
			using ComeHomeCallback = std::function<void( int )>;

			BulletManager();

		public:
			static BulletManagerUp create( const int caching_amount );

		private:
			bool Init( const int caching_amount );

		public:
			ComeHomeCallback GetComeHomeCallback();
			int GetIdleTarget();
			void ComeHomeTarget( const int target_index );
			void Refill();

		private:
			ContainerT mIdleTarget;
			ContainerT mRestTarget;
		};
	}
}
