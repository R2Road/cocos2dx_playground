#pragma once

#include <algorithm>
#include <cstdlib>

#include "cpg_Point.h"

namespace algorithm_practice_astar
{
	class Node4AStar
	{
	public:
		enum class eStatus
		{
			None,
			Open,
			Close,
		};

		Node4AStar( const cpg::Point current_point, const cpg::Point previous_point, const cpg::Point start_point, const cpg::Point end_point ) :
			mStatus( eStatus::Open )
			, mCurrentPoint( current_point )
			, mPreviousPoint( previous_point )
			, mCost2Start( calculateCost( current_point, start_point ) )
			, mCost2End( calculateCost( current_point, end_point ) )
			, mTotalCost( mCost2Start + mCost2End )
		{}

		//
		// Getter
		//
		eStatus GetStatus() const { return mStatus; }
		cpg::Point GetPoint() const { return mCurrentPoint; }
		cpg::Point GetPreviousPoint() const { return mPreviousPoint; }
		int GetCost2Start() const { return mCost2Start; }
		int GetCost2End() const { return mCost2End; }
		int GetTotalCost() const { return mTotalCost; }

		void Clear()
		{
			mStatus = eStatus::None;
			mCost2Start = 0;
			mCost2End = 0;
			mTotalCost = 0;
		};
		bool IsValid() const
		{
			return eStatus::None != mStatus;
		}

		void Close()
		{
			mStatus = eStatus::Close;
		}

	private:
		int calculateCost( const cpg::Point point_1, const cpg::Point point_2 )
		{
			const auto distance_x = std::abs( point_1.x - point_2.x );
			const auto distance_y = std::abs( point_1.y - point_2.y );

			const auto gap = std::abs( distance_x - distance_y );

			const auto max_value = std::max( distance_x, distance_y );
			const auto diagonal = max_value - gap;

			return ( gap * 10 ) + ( diagonal * 14 );
		}

	private:
		eStatus mStatus;
		cpg::Point mCurrentPoint;
		cpg::Point mPreviousPoint;
		int mCost2Start;
		int mCost2End;
		int mTotalCost;
	};
}
