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

	class DirectionCell
	{
	public:
		DirectionCell();

		void Clear() { mTotalDirection = cpg::Direction4::eState::None; };
		void Begin( const cpg::Direction4 parent_direction );

		char GetTotalDirection() const { return mTotalDirection; }
		cpg::Direction4 GetCurrentDirection() const { return mCurrentDirection.GetState(); }

		bool HasDirection() const { return 0 != mTotalDirection; }
		cpg::Point PopDirection();

		void RotateCurrentDirection( const bool rotate_right );

	private:
		char mTotalDirection;
		cpg::Direction4 mCurrentDirection;
	};
}
