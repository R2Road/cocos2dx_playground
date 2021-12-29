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

		Node4AStar() :
			mStatus( eStatus::None )
			, mCost2Start( 0 )
			, mCost2End( 0 )
			, mTotalCost( 0 )
		{}

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

		void Open( const cpg::Point current_point, const cpg::Point start_point, const cpg::Point end_point )
		{
			mStatus = eStatus::Open;

			mCost2Start = calculateCost( current_point, start_point );
			mCost2End = calculateCost( current_point, end_point );
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
		int mCost2Start;
		int mCost2End;
		int mTotalCost;
	};
}
