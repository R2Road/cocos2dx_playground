#pragma once

#include "cpg_Point.h"

namespace cpg
{
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

		cpg::Point GetPoint() const;

	private:
		eState mState;
	};
}
