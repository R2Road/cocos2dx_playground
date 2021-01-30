#include "algorithm_practice_floodfill_Constant.h"

namespace algorithm_practice_floodfill
{
	Direction4::Direction4() : mState( eState::Up ) {}
	Direction4::Direction4( const eState state ) : mState( state ) {}

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


	DirectionCell::DirectionCell() : mTotalDirection( Direction4::eState::None ), mCurrentDirection( Direction4::eState::FIRST )
	{}

	void DirectionCell::Begin( const Direction4::eState parent_direction )
	{
		mTotalDirection = Direction4::eState::ALL;

		if( Direction4::eState::None == parent_direction )
		{
			mCurrentDirection = Direction4::eState::FIRST;
		}
		else
		{
			mCurrentDirection = parent_direction;
		}
	}

	cpg::Point DirectionCell::PopDirection()
	{
		cpg::Point out_point;

		if( mTotalDirection & mCurrentDirection.GetState() )
		{
			mTotalDirection ^= mCurrentDirection.GetState();

			switch( mCurrentDirection.GetState() )
			{
			case Direction4::eState::Up:
				out_point = cpg::Point{ 0, 1 };
				break;
			case Direction4::eState::Right:
				out_point = cpg::Point{ 1, 0 };
				break;
			case Direction4::eState::Down:
				out_point = cpg::Point{ 0, -1 };
				break;
			case Direction4::eState::Left:
				out_point = cpg::Point{ -1, 0 };
				break;

			default:
				out_point = cpg::Point{ 0, 0 };
				break;
			}
		}
		else
		{
			out_point = cpg::Point{ 0, 0 };
		}

		return out_point;
	}

	void DirectionCell::RotateCurrentDirection( const bool rotate_right )
	{
		mCurrentDirection.Rotate( rotate_right );
	}
}
