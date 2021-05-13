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
		void operator+=( const Point& p )
		{
			x += p.x;
			y += p.y;
		}

		Point operator+( const Point& p ) const
		{
			return Point{ x + p.x, y + p.y };
		}
		Point operator-( const Point& p ) const
		{
			return Point{ x - p.x, y - p.y };
		}

		int x = 0;
		int y = 0;
	};
}
