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

	cpg::Point Direction4::GetPoint() const
	{
		cpg::Point out_point;

		switch( mState )
		{
		case cpg::Direction4::eState::Up:
			out_point = cpg::Point{ 0, 1 };
			break;
		case cpg::Direction4::eState::Right:
			out_point = cpg::Point{ 1, 0 };
			break;
		case cpg::Direction4::eState::Down:
			out_point = cpg::Point{ 0, -1 };
			break;
		case cpg::Direction4::eState::Left:
			out_point = cpg::Point{ -1, 0 };
			break;

		default:
			out_point = cpg::Point{ 0, 0 };
			break;
		}

		return out_point;
	}
}
