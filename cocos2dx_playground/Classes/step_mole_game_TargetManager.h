#pragma once

#include <functional>
#include <list>
#include <memory>

#include "step_mole_game_StageConfig.h"

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
			static TargetManagerUp create( const StageConfig& stage_config );

		private:
			bool init( const StageConfig& stage_config );
			void shuffle( ContainerT& target_container );

		public:
			int GetIdleTarget();
			void ComeHomeTarget( const int target_index );
			void Refill();

		private:
			ContainerT mIdleTarget;
			ContainerT mRestTarget;
		};
	}
}
