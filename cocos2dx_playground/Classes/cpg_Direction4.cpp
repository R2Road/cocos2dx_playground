#include "cpg_Direction4.h"

namespace cpg
{
	Direction4::Direction4() : mState( eState::Up )
	{}
	Direction4::Direction4( const eState state ) : mState( state )
	{}

	void Direction4::Rotate( const bool rotate_right )
	{
		char new_state = 0;
		if( rotate_right )
		{
			new_state = mState << 1;
			if( eState::LAST < new_state )
			{
				new_state = eState::FIRST;
			}
		}
		else
		{
			new_state = mState >> 1;
			if( eState::None >= new_state )
			{
				new_state = eState::LAST;
			}
		}

		mState = static_cast<eState>( new_state );
	}
}
