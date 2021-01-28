#include "algorithm_practice_floodfill_Constant.h"

namespace algorithm_practice_floodfill
{
	DirectionCell::DirectionCell() : mCurrentDirection( eDirectionType::None )
	{}

	void DirectionCell::Begin( const eDirectionType parent_direction )
	{
		mCurrentDirection = eDirectionType::ALL ^ parent_direction;
	}

	cpg::Point DirectionCell::PopNextDirection()
	{
		cpg::Point out_point;

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
			mCurrentDirection = 0;
			out_point = cpg::Point{ 0, 0 };
			break;
		}

		mCurrentDirection = mCurrentDirection << 1;
		return out_point;
	}
}
