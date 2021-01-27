#pragma once

#include <string>

#include "algorithm_practice_floodfill_Constant.h"
#include "cpg_Grid.h"
#include "cpg_Point.h"

namespace algorithm_practice_floodfill
{
	class Grid4Floodfill
	{
	public:
		using GridT = cpg::Grid<Cell>;

		explicit Grid4Floodfill();

		void Reset( const std::size_t new_width, const std::size_t new_height );

		//
		// Iteration
		//
		GridT::ConstIteratorT begin() const { return mGrid.begin(); }
		GridT::ConstIteratorT end() const { return mGrid.end(); }
		GridT::IteratorT begin() { return mGrid.begin(); }
		GridT::IteratorT end() { return mGrid.end(); }

		//
		// Getter
		//
		std::size_t GetWidth() const { return mGrid.GetWidth(); }
		std::size_t GetHeight() const { return mGrid.GetHeight(); }
		const cpg::Point& GetEntryPoint() const { return mEntryPoint; }
		eCellType GetCellType( const std::size_t x, const std::size_t y );

		//
		// Setter
		//
		void SetEntryPoint( const cpg::Point& new_entry_point );
		void SetCellType( const std::size_t x, const std::size_t y, const eCellType cell_type );

		//
		// File
		//
		void ExportJsonString( std::string& out_json_string ) const;
		bool LoadJsonString( const std::string& json_string );

	private:
		cpg::Point mEntryPoint;
		GridT mGrid;
	};
}
