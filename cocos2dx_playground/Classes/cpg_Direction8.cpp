#include "cpg_Direction8.h"

namespace cpg
{
	Direction8::Direction8() : mState( eState::Up )
	{}
	Direction8::Direction8( const eState state ) : mState( state )
	{}

	void Direction8::Rotate( const bool rotate_right )
	{
		short new_state = 0;
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
	void Direction8::Rotate( const bool rotate_right, const int count )
	{
		for( int i = 0; count > i; ++i )
		{
			Rotate( rotate_right );
		}
	}

	cpg::Point Direction8::GetPoint() const
	{
		cpg::Point out_point;

		switch( mState )
		{
		case cpg::Direction8::eState::Up:
			out_point = cpg::Point{ 0, 1 };
			break;
		case cpg::Direction8::eState::UpRight:
			out_point = cpg::Point{ 1, 1 };
			break;
		case cpg::Direction8::eState::Right:
			out_point = cpg::Point{ 1, 0 };
			break;
		case cpg::Direction8::eState::DownRight:
			out_point = cpg::Point{ 1, -1 };
			break;
		case cpg::Direction8::eState::Down:
			out_point = cpg::Point{ 0, -1 };
			break;
		case cpg::Direction8::eState::DownLeft:
			out_point = cpg::Point{ -1, -1 };
			break;
		case cpg::Direction8::eState::Left:
			out_point = cpg::Point{ -1, 0 };
			break;
		case cpg::Direction8::eState::UpLeft:
			out_point = cpg::Point{ -1, 1 };
			break;

		default:
			out_point = cpg::Point{ 0, 0 };
			break;
		}

		return out_point;
	}
}
