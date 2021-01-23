#pragma once

#include "cpg_Point.h"

namespace algorithm_practice_floodfill
{
	enum eToolIndex
	{
		Wall,
		Road,
		Entry,
	};

	enum class eGridType
	{
		Road,
		Wall,
	};

	enum eDirectionType
	{
		Up = 1,
		Right = 1 << 1,
		Down = 1 << 2,
		Left = 1 << 3,

		FIRST = Up,
	};

	class GridDirection
	{
	public:
		GridDirection();

		bool HasDirection() const { return 0 != mCurrentDirection; }
		cpg::Point PopNextDirection();

	private:
		char mCurrentDirection;
	};

	struct Cell
	{
		eGridType Type = eGridType::Road;
		GridDirection Direction;
	};
}
