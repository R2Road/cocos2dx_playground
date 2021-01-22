#pragma once

#include <vector>

#include "cpg_GridIndexConverter.h"

namespace cpg
{
	template<typename T>
	class Grid
	{
	public:
		using ValueT = T;
		using ContainerT = std::vector<ValueT>;

		Grid( const std::size_t width, const std::size_t height ) :
			mWidth( width )
			, mHeight( height )
			, mContainer( width * height )
			, mIndexConverter( width, height )
		{}

		virtual ~Grid() {}

	public:
		std::size_t GetWidth() const { return mWidth; }
		std::size_t GetHeight() const { return mHeight; }
		const ContainerT& GetContainer() const { return mContainer; }

		const ValueT& Get( const std::size_t linear_idx ) const
		{
			if( linear_idx >= static_cast<int>( mContainer.size() ) )
			{
				static const ValueT dummy;
				return dummy;
			}

			return mContainer[linear_idx];
		}
		const ValueT& Get( const std::size_t x, const std::size_t y ) const
		{
			return Get( mIndexConverter.To_Linear( x, y ) );
		}


		void Set( const std::size_t linear_idx, const ValueT& new_value )
		{
			if( linear_idx >= static_cast<int>( mContainer.size() ) )
			{
				return;
			}

			mContainer[linear_idx] = new_value;
		}
		void Set( const std::size_t x, const std::size_t y, const ValueT& new_value )
		{
			Set(
				mIndexConverter.To_Linear( x, y )
				, new_value
			);
		}

	private:
		std::size_t mWidth;
		std::size_t mHeight;
		ContainerT mContainer;

		cpg::GridIndexConverter mIndexConverter;
	};
}