#pragma once

#include <array>

#include "cpg_Point.h"

namespace algorithm_practice_loophero
{
	const unsigned int GRID_WIDTH = 21;
	const unsigned int GRID_HEIGHT = 14;

	const unsigned int ROAD_LENGTH = 34;
	const unsigned int ROAD_PIVOT_COUNT = 4;

	const std::array<cpg::Point, static_cast<std::size_t>( ROAD_PIVOT_COUNT )> PIVOT_LIST = { {
		{ 0, 1 }
		, { 1, 1 }
		, { 1, 0 }
		, { 0, 0 }
	} };
}
