#pragma once

#include "cpg_Point.h"
#include "cpg_Direction4.h"

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

	class Cell4FloodFill
	{
	public:
		Cell4FloodFill();

		void Clear() { mParentPoint = { -1, -1 }, mTotalDirection = cpg::Direction4::eState::None; };
		void Begin( const cpg::Point parent_point, const cpg::Direction4 parent_direction );

		cpg::Point GetParentPoint() const { return mParentPoint; }

		char GetTotalDirection() const { return mTotalDirection; }
		cpg::Direction4 GetCurrentDirection() const { return mCurrentDirection.GetState(); }

		bool HasDirection() const { return 0 != mTotalDirection; }
		cpg::Point PopDirection();

		void RotateCurrentDirection( const bool rotate_right );

	private:
		cpg::Point mParentPoint;
		char mTotalDirection;
		cpg::Direction4 mCurrentDirection;
	};
}
