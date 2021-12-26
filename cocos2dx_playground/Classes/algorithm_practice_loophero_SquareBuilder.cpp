#include "algorithm_practice_loophero_SquareBuilder.h"

#include "base/ccMacros.h"

#include "cpg_Clamp.h"
#include "cpg_Random.h"

#include "algorithm_practice_loophero_Constant.h"

namespace algorithm_practice_loophero
{
	cpg::Point SquareBuilder::Build( ContainerT* out_container )
	{
		// 0. Ready
		const int required_road_count = algorithm_practice_loophero::ROAD_LENGTH - algorithm_practice_loophero::ROAD_PIVOT_COUNT;

		// 1. Adjust Pivot List
		auto CurrentPivotList = algorithm_practice_loophero::PIVOT_LIST;
		cpg::Point square_size;
		{
			//
			// # "required_half_road_count" contained '+' area
			// # we need 'width" and 'height'
			//
			//  p+++++++++++p
			//  +           -
			//  +           -
			//  +           -
			//  p-----------p
			//
			const int required_half_road_count = ( algorithm_practice_loophero::ROAD_LENGTH - algorithm_practice_loophero::ROAD_PIVOT_COUNT ) / 2;

			const auto square_width_without_pivot = cpg::Random::GetInt( static_cast<int>( required_half_road_count * 0.3f ), static_cast<int>( required_half_road_count * 0.9f ) );

			square_size.x = required_half_road_count - square_width_without_pivot > square_width_without_pivot ? required_half_road_count - square_width_without_pivot : square_width_without_pivot;
			square_size.y = required_half_road_count - square_size.x;

			CurrentPivotList[1].x += square_size.x;
			CurrentPivotList[2].x += square_size.x;

			CurrentPivotList[0].y += square_size.y;
			CurrentPivotList[1].y += square_size.y;
		}

		// 2. Make Road
		out_container->clear();
		for( int i = 0; ROAD_PIVOT_COUNT > i; ++i )
		{
			const auto start_point = CurrentPivotList[i];

			//
			// # i + 1 mean
			// # chain of pivot
			//
			// 0 > 1 > 2 > 3 > 0
			//
			const auto end_point = i + 1 < ROAD_PIVOT_COUNT ? CurrentPivotList[i + 1] : CurrentPivotList[0];


			const auto start2end = end_point - start_point;

			//
			// Normalize??
			//
			const cpg::Point dir{
				cpg::clamp( start2end.x, -1, 1 )
				, cpg::clamp( start2end.y, -1, 1 )
			};

			auto cur_point = start_point;
			do
			{
				out_container->push_back( cur_point );

				cur_point += dir;
			} while( end_point != cur_point );
		}

		// 3. Find Max Point
		int max_x = 0;
		int max_y = 0;
		for( const auto& p : *out_container )
		{
			if( p.x > max_x )
			{
				max_x = p.x;
			}

			if( p.y > max_y )
			{
				max_y = p.y;
			}
		}

		// 4. Align
		const int move_x = ( GRID_WIDTH - 1 - max_x ) / 2;
		const int move_y = ( GRID_HEIGHT - 1 - max_y ) / 2;
		for( auto& p : *out_container )
		{
			p.x += move_x;
			p.y += move_y;
		}

		CCASSERT( ROAD_LENGTH == out_container->size(), "Not Enough Road Length" );

		return square_size;
	}
}
