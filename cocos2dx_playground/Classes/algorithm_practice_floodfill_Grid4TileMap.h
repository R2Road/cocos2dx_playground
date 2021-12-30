#pragma once

#include <string>

#include "cpg_Grid.h"
#include "cpg_Point.h"

namespace algorithm_practice_floodfill
{
	enum class eCellType
	{
		Road,
		Wall,
	};

	class Grid4TileMap
	{
	public:
		using GridT = cpg::Grid<eCellType>;

		explicit Grid4TileMap();

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
		eCellType GetCellType( const std::size_t x, const std::size_t y ) const;

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
