#pragma once

namespace cpg
{
	struct Point
	{
		bool operator==( const Point& p ) const
		{
			return x == p.x && y == p.y;
		}
		bool operator!=( const Point& p ) const
		{
			return !( *this == p );
		}

		int x = 0;
		int y = 0;
	};
}
