#pragma once

#include "cpg_Point.h"

namespace cpg
{
	class Direction8
	{
	public:
		enum eState
		{
			None = 0,
			Up = 1,
			UpRight = 1 << 1,
			Right = 1 << 2,
			DownRight = 1 << 3,
			Down = 1 << 4,
			DownLeft = 1 << 5,
			Left = 1 << 6,
			UpLeft = 1 << 7,

			FIRST = Up,
			LAST = UpLeft,
			ALL = Up | UpRight | Right | DownRight | Down | DownLeft | Left | UpLeft,
		};

		Direction8();
		Direction8( const eState state );

		eState GetState() const { return mState; }
		void SetState( eState state ) { mState = state; }

		void Rotate( const bool rotate_right );
		void Rotate( const bool rotate_right, const int count );

		cpg::Point GetPoint() const;

	private:
		eState mState;
	};
}
