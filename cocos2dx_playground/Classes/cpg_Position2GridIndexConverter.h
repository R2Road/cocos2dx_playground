#pragma once

namespace cpg
{
	class Position2GridIndexConverter
	{
	public:
		struct Point
		{
			int x = 0;
			int y = 0;
		};

		Position2GridIndexConverter( const int block_width, const int block_height );

		Point Position2Point( const float x, const float y ) const;

	private:
		int mBlockWidth;
		int mBlockHeight;
	};
}
