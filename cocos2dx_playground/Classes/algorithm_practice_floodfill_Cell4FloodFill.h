#pragma once

#include "cpg_Point.h"
#include "cpg_Direction4.h"

namespace algorithm_practice_floodfill
{
	class Cell4FloodFill
	{
	public:
		Cell4FloodFill() :
			mValid( false )
			, mParentPoint( { -1, -1 } )
			, mTotalDirection( cpg::Direction4::eState::None )
			, mCurrentDirection( cpg::Direction4::eState::FIRST )
		{}

		//
		// Getter
		//
		cpg::Point GetParentPoint() const { return mParentPoint; }
		char GetTotalDirection() const { return mTotalDirection; }
		cpg::Direction4 GetCurrentDirection() const { return mCurrentDirection.GetState(); }

		bool IsValid() const { return mValid; }

		void Clear()
		{
			mValid = false;
			mParentPoint = { -1, -1 };
			mTotalDirection = cpg::Direction4::eState::None;
		};
		void Begin( const cpg::Point parent_point, const cpg::Direction4 parent_direction )
		{
			mValid = true;

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
		
		bool HasDirection() const
		{
			return 0 != mTotalDirection;
		}
		cpg::Direction4 PopDirection()
		{
			cpg::Direction4 out_direction( cpg::Direction4::None );

			if( HasDirection() )
			{
				while( 0 == ( mTotalDirection & mCurrentDirection.GetState() ) )
				{
					mCurrentDirection.Rotate( true );
				}

				mTotalDirection ^= mCurrentDirection.GetState();

				out_direction = mCurrentDirection;
			}

			return out_direction;
		}
		void RotateCurrentDirection( const bool rotate_right )
		{
			mCurrentDirection.Rotate( rotate_right );
		}

	private:
		bool mValid;
		cpg::Point mParentPoint;
		char mTotalDirection;
		cpg::Direction4 mCurrentDirection;
	};
}
