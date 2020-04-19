#pragma once

#include <functional>
#include <list>
#include <memory>

namespace step_mole
{
	namespace game
	{
		using TargetManagerUp = std::unique_ptr<class TargetManager>;

		class TargetManager
		{
		private:
			using ValueT = int;
			using ContainerT = std::list<ValueT>;
			using RestRequestCallback = std::function<void( ValueT )>;

			TargetManager();

		public:
			static TargetManagerUp create( const int width, const int height );

		private:
			bool Init( const int width, const int height );

		public:
			int GetIdleTarget();

		private:
			ContainerT mIdleTarget;
		};
	}
}
