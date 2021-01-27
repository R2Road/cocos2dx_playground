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
		using IteratorT = typename std::vector<ValueT>::iterator;
		using ConstIteratorT = typename std::vector<ValueT>::const_iterator;

		Grid() :
			mWidth( 1 )
			, mHeight( 1 )
			, mContainer( 1 * 1 )
			, mIndexConverter( 1, 1 )
		{}

		virtual ~Grid() {}

	public:
		std::size_t GetWidth() const { return mWidth; }
		std::size_t GetHeight() const { return mHeight; }

		//
		// Iteration
		//
		ConstIteratorT begin() const { return mContainer.begin(); }
		ConstIteratorT end() const { return mContainer.end(); }
		IteratorT begin() { return mContainer.begin(); }
		IteratorT end() { return mContainer.end(); }

		void Reset( const std::size_t new_width, const std::size_t new_height )
		{
			mWidth = new_width;
			mHeight = new_height;

			mContainer.clear();
			mContainer.resize( mWidth *  mHeight );

			mIndexConverter = cpg::GridIndexConverter( mWidth, mHeight );
		}

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