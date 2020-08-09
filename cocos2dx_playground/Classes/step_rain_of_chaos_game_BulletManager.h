#pragma once

#include <functional>
#include <list>
#include <memory>
#include <vector>

namespace step_rain_of_chaos
{
	namespace game
	{
		using BulletManagerUp = std::unique_ptr<class BulletManager>;

		class BulletManager
		{
		private:
			using ValueT = int;
			using BaseContainerT = std::vector<ValueT>;
			using ComeHomeCallback = std::function<void( int )>;
			using LiveContainerT = std::list<ValueT>;

			BulletManager();

		public:
			~BulletManager();

		public:
			static BulletManagerUp create();

			const LiveContainerT& GetLiveTargetContainer() const { return mLiveTargetContainer; }
			ComeHomeCallback GetComeHomeCallback();

			void RequestGenerate( const int amount );
			int GetIdleTarget();

		private:
			void ComeHomeTarget( const int target_index );
			bool Refill();

		private:
			int mBulletAmount;
			BaseContainerT mIdleTarget;
			BaseContainerT mRestTarget;
			LiveContainerT mLiveTargetContainer;
		};
	}
}
