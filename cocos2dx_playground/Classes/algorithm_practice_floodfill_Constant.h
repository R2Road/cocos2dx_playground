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

	enum class eCellType
	{
		Road,
		Wall,
	};

	enum eDirectionType
	{
		None = 0,
		Up = 1,
		Right = 1 << 1,
		Down = 1 << 2,
		Left = 1 << 3,

		FIRST = Up,
		ALL = Up | Right | Down | Left,
	};

	class DirectionCell
	{
	public:
		DirectionCell();

		void Clear() { mCurrentDirection = eDirectionType::None; };
		void Begin( const eDirectionType parent_direction );

		char GetTotalDirection() const { return mCurrentDirection; }
		bool HasDirection() const { return 0 != mCurrentDirection; }
		cpg::Point PopNextDirection();

	private:
		char mCurrentDirection;
	};
}
