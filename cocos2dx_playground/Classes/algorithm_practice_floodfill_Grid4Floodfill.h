#pragma once

#include "algorithm_practice_floodfill_Constant.h"
#include "cpg_Grid.h"

namespace algorithm_practice_floodfill
{
	class Grid4Floodfill : public cpg::Grid<GridValue>
	{
	public:
		explicit Grid4Floodfill( const std::size_t width, const std::size_t height );
	};
}
