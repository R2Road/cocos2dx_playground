#include "cpg_Position2GridIndexConverter.h"

#include <algorithm>

namespace cpg
{
	Position2GridIndexConverter::Position2GridIndexConverter( const int block_width, const int block_height ) :
		mBlockWidth( block_width )
		, mBlockHeight( block_height )
	{}


	Position2GridIndexConverter::Point Position2GridIndexConverter::Position2Point( const float x, const float y ) const
	{
		return Point{
			static_cast<int>( x / mBlockWidth )
			, static_cast<int>( y / mBlockHeight )
		};
	}
}
