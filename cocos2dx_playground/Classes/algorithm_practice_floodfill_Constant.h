#pragma once

#include "cpg_Point.h"
#include "cpg_Direction4.h"

namespace algorithm_practice_floodfill
{
	class Cell4FloodFill
	{
	public:
		Cell4FloodFill();

		void Clear() { mValid = false; mParentPoint = { -1, -1 }; mTotalDirection = cpg::Direction4::eState::None; };
		void Begin( const cpg::Point parent_point, const cpg::Direction4 parent_direction );

		bool IsValid() const { return mValid; }

		cpg::Point GetParentPoint() const { return mParentPoint; }

		char GetTotalDirection() const { return mTotalDirection; }
		cpg::Direction4 GetCurrentDirection() const { return mCurrentDirection.GetState(); }

		bool HasDirection() const { return 0 != mTotalDirection; }
		cpg::Direction4 PopDirection();

		void RotateCurrentDirection( const bool rotate_right );

	private:
		bool mValid;
		cpg::Point mParentPoint;
		char mTotalDirection;
		cpg::Direction4 mCurrentDirection;
	};
}
