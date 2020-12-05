#pragma once

namespace cpg
{
	class GridIndexConverter
	{
	public:
		struct Point
		{
			int x = 0;
			int y = 0;
		};

		GridIndexConverter( const int width, const int height );

		Point To_Point( const int linear_index ) const;
		int To_Linear( const Point point_index ) const;
		int To_Linear( const int x, const int y ) const;

		Point Position2Point( const float x, const float y ) const;

	private:
		int mWidth;
		int mHeight;
		int mLinearEnd;
	};
}
