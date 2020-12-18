#pragma once

#include <vector>

#include "step_flipflip_game_Constant.h"

namespace step_flipflip
{
	namespace game
	{
		struct StageConfig;

		class StageData
		{
		public:
			using ContainerT = std::vector<eCardType>;

			StageData();

			const ContainerT& GetContainer() const { return mContainer; }

			bool Reset( const int width, const int height );

		private:
			ContainerT mContainer;
		};
	}
}
