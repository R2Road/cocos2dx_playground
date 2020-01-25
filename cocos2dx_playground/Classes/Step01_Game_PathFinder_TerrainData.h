#pragma once

#include <vector>

namespace step01
{
	namespace game
	{
		namespace pathfinder
		{
			class TerrainData
			{
			public:
				TerrainData();

				void load( const int width, const int height );

				int getWidth() const { return mWidth; }
				int getHeight() const { return mHeight; }

				const bool get( const std::size_t x, const std::size_t y ) const;
				const std::vector<bool> get() const { return container; }

			private:
				int mWidth;
				int mHeight;
				std::vector<bool> container;
			};
		} // namespace pathfinder
	}
}
