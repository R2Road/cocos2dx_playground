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

	class Direction4
	{
	public:
		enum eState
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

		Direction4();
		Direction4( const eState state );

		eState GetState() const { return mState; }
		void SetState( eState state ) { mState = state; }

		void Rotate( const bool rotate_right );

	private:
		eState mState;
	};

	class DirectionCell
	{
	public:
		DirectionCell();

		void Clear() { mTotalDirection = Direction4::eState::None; };
		void Begin( const Direction4 parent_direction );

		char GetTotalDirection() const { return mTotalDirection; }
		Direction4 GetCurrentDirection() const { return mCurrentDirection.GetState(); }

		bool HasDirection() const { return 0 != mTotalDirection; }
		cpg::Point PopDirection();

		void RotateCurrentDirection( const bool rotate_right );

	private:
		char mTotalDirection;
		Direction4 mCurrentDirection;
	};
}
