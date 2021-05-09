#pragma once

#include "cpg_Point.h"

namespace algorithm_practice_loophero
{
	const unsigned int GRID_WIDTH = 21;
	const unsigned int GRID_HEIGHT = 14;

	const unsigned int LOAD_LENGTH = 34;
	const unsigned int LOAD_PIVOT_COUNT = 4;

	const cpg::Point PIVOT_LIST[LOAD_PIVOT_COUNT] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 } };
}
