#pragma once

#include <string>

#include "algorithm_practice_floodfill_Constant.h"
#include "cpg_Grid.h"
#include "cpg_Point.h"

namespace algorithm_practice_floodfill
{
	class Grid4Floodfill : public cpg::Grid<GridValue>
	{
	public:
		explicit Grid4Floodfill( const std::size_t width, const std::size_t height );

		const cpg::Point& GetEntryPoint() const { return mEntryPoint; }
		void SetEntryPoint( const cpg::Point& new_entry_point );

		void ExportJsonString( std::string& out_json_string ) const;
		bool LoadJsonString( const std::string& json_string );

	private:
		cpg::Point mEntryPoint;
	};
}
