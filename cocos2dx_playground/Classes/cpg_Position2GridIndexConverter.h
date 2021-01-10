#pragma once

#include "cpg_Point.h"

namespace cpg
{
	class Position2GridIndexConverter
	{
	public:
		Position2GridIndexConverter( const int block_width, const int block_height );

		Point Position2Point( const float x, const float y ) const;

	private:
		int mBlockWidth;
		int mBlockHeight;
	};
}
