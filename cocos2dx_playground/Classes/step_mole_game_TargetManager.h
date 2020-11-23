#pragma once

#include <vector>
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
			using ContainerT = std::vector<ValueT>;

			TargetManager();

		public:
			static TargetManagerUp create( const StageConfig& stage_config );

		private:
			void init( const StageConfig& stage_config );

		public:
			int GetIdleTarget();
			void ComeHomeTarget( const int target_index );
			void Refill();

		private:
			ContainerT mIdleTargetContainer;
			ContainerT::const_iterator mIdleTargetIndicator;
			ContainerT mRestTargetContainer;
		};
	}
}
