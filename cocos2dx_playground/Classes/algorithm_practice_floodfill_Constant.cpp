#include "algorithm_practice_floodfill_Constant.h"

namespace algorithm_practice_floodfill
{
	Direction4::Direction4() : mState( eDirectionType::Up ) {}
	Direction4::Direction4( const eDirectionType state ) : mState( state ) {}

	void Direction4::Rotate( const bool rotate_right )
	{
		char new_state = 0;
		if( rotate_right )
		{
			new_state = mState << 1;
			if( eDirectionType::LAST < new_state )
			{
				new_state = eDirectionType::FIRST;
			}
		}
		else
		{
			new_state = mState >> 1;
			if( eDirectionType::None >= new_state )
			{
				new_state = eDirectionType::LAST;
			}
		}

		mState = static_cast<eDirectionType>( new_state );
	}


	DirectionCell::DirectionCell() : mTotalDirection( eDirectionType::None ), mCurrentDirection( eDirectionType::FIRST )
	{}

	void DirectionCell::Begin( const eDirectionType parent_direction )
	{
		mTotalDirection = eDirectionType::ALL;

		if( eDirectionType::None == parent_direction )
		{
			mCurrentDirection = eDirectionType::FIRST;
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
			case eDirectionType::Up:
				out_point = cpg::Point{ 0, 1 };
				break;
			case eDirectionType::Right:
				out_point = cpg::Point{ 1, 0 };
				break;
			case eDirectionType::Down:
				out_point = cpg::Point{ 0, -1 };
				break;
			case eDirectionType::Left:
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
