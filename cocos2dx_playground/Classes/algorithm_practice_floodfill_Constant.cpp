#include "algorithm_practice_floodfill_Constant.h"

namespace algorithm_practice_floodfill
{
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

		if( mTotalDirection & mCurrentDirection )
		{
			mTotalDirection ^= mCurrentDirection;

			switch( mCurrentDirection )
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

	void DirectionCell::RotateCurrentDirection( const bool rotate_right = true )
	{
		char new_direction = 0;
		if( rotate_right )
		{
			new_direction = mCurrentDirection << 1;
			if( eDirectionType::LAST < new_direction )
			{
				new_direction = eDirectionType::FIRST;
			}
		}
		else
		{
			new_direction = mCurrentDirection >> 1;
			if( eDirectionType::None >= new_direction )
			{
				new_direction = eDirectionType::LAST;
			}
		}

		mCurrentDirection = static_cast<eDirectionType>( new_direction );
	}
}
