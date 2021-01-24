#include "cpg_GridIndexConverter.h"

#include <algorithm>

namespace cpg
{
	GridIndexConverter::GridIndexConverter( const int width, const int height ) :
		mWidth( width ), mHeight( height )
		, mLinearEnd( To_Linear( width, height ) )
	{}

	cpg::Point GridIndexConverter::To_Point( const int linear_index ) const
	{
		const int fixed_linear_index = std::min( std::max( 0, linear_index ), mLinearEnd );

		const int y = fixed_linear_index / mWidth;
		const int x = fixed_linear_index - ( y * mWidth );
		return cpg::Point{ x, y };
	}
	int GridIndexConverter::To_Linear( const cpg::Point point_index ) const
	{
		return To_Linear( point_index.x, point_index.y);
	}
	int GridIndexConverter::To_Linear( const int x, const int y ) const
	{
		const int fixed_x = std::min( std::max( 0, x ), mWidth - 1 );
		const int fixed_y = std::min( std::max( 0, y ), mHeight - 1 );

		return fixed_x + ( fixed_y * mWidth );
	}
}
