#include "algorithm_practice_floodfill_Constant.h"

namespace algorithm_practice_floodfill
{
	Cell4FloodFill::Cell4FloodFill() : mParentPoint( { -1, -1 } ), mTotalDirection( cpg::Direction4::eState::None ), mCurrentDirection( cpg::Direction4::eState::FIRST )
	{}

	void Cell4FloodFill::Begin( const cpg::Point parent_point, const cpg::Direction4 parent_direction )
	{
		mParentPoint = parent_point;

		if( cpg::Direction4::eState::None == parent_direction.GetState() )
		{
			mCurrentDirection = cpg::Direction4::eState::FIRST;
		}
		else
		{
			mCurrentDirection = parent_direction;
		}

		mTotalDirection = cpg::Direction4::eState::ALL;

		auto temp_direction4 = parent_direction;
		temp_direction4.Rotate( true );
		temp_direction4.Rotate( true );
		mTotalDirection ^= temp_direction4.GetState();
	}

	cpg::Point Cell4FloodFill::PopDirection()
	{
		cpg::Point out_point;

		if( mTotalDirection & mCurrentDirection.GetState() )
		{
			mTotalDirection ^= mCurrentDirection.GetState();

			switch( mCurrentDirection.GetState() )
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
		}
		else
		{
			out_point = cpg::Point{ 0, 0 };
		}

		return out_point;
	}

	void Cell4FloodFill::RotateCurrentDirection( const bool rotate_right )
	{
		mCurrentDirection.Rotate( rotate_right );
	}
}
