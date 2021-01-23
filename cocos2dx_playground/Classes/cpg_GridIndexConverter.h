#pragma once

#include "cpg_Point.h"

namespace cpg
{
	class GridIndexConverter
	{
	public:
		GridIndexConverter( const int width, const int height );

		cpg::Point To_Point( const int linear_index ) const;
		int To_Linear( const cpg::Point point_index ) const;
		int To_Linear( const int x, const int y ) const;

	private:
		int mWidth;
		int mHeight;
		int mLinearEnd;
	};
}
