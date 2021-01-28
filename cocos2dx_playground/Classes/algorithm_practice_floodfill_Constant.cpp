#include "algorithm_practice_floodfill_Constant.h"

namespace algorithm_practice_floodfill
{
	CellDirection::CellDirection() : mCurrentDirection( eDirectionType::None )
	{}

	void CellDirection::Reset()
	{
		mCurrentDirection = eDirectionType::FIRST;
	}

	cpg::Point CellDirection::PopNextDirection()
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
