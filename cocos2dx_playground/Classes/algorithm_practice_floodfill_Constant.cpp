#include "algorithm_practice_floodfill_Constant.h"

namespace algorithm_practice_floodfill
{
	CellDirection::CellDirection() : mTotalDirection( eDirectionType::None )
	{}

	void CellDirection::Begin( const eDirectionType parent_direction )
	{
		mTotalDirection = eDirectionType::ALL ^ parent_direction;
	}

	cpg::Point CellDirection::PopNextDirection()
	{
		cpg::Point out_point;

		switch( mTotalDirection )
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
			mTotalDirection = 0;
			out_point = cpg::Point{ 0, 0 };
			break;
		}

		mTotalDirection = mTotalDirection << 1;
		return out_point;
	}
}
