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
		LAST = Left,
		ALL = Up | Right | Down | Left,
	};

	class Direction4
	{
	public:
		Direction4();
		Direction4( const eDirectionType state );

		eDirectionType GetState() const { return mState; }
		void SetState( eDirectionType state ) { mState = state; }

		void Rotate( const bool rotate_right );

	private:
		eDirectionType mState;
	};

	class DirectionCell
	{
	public:
		DirectionCell();

		void Clear() { mTotalDirection = eDirectionType::None; };
		void Begin( const eDirectionType parent_direction );

		char GetTotalDirection() const { return mTotalDirection; }
		eDirectionType GetCurrentDirection() const { return mCurrentDirection.GetState(); }

		bool HasDirection() const { return 0 != mTotalDirection; }
		cpg::Point PopDirection();

		void RotateCurrentDirection( const bool rotate_right );

	private:
		char mTotalDirection;
		Direction4 mCurrentDirection;
	};
}
