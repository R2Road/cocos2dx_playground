#pragma once

#include <vector>

#include "cpg_GridIndexConverter.h"
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
			eCardType Get( const int x, const int y ) const;

			bool Reset( const int width, const int height );

		private:
			cpg::GridIndexConverter mIndexConverter;
			ContainerT mContainer;
		};
	}
}
