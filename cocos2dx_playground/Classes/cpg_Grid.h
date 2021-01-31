#pragma once

#include <vector>

#include "cpg_GridIndexConverter.h"

namespace cpg
{
	template<typename T>
	class Grid
	{
	public:
		using CellT = T;
		using ContainerT = std::vector<CellT>;
		using IteratorT = typename ContainerT::iterator;
		using ConstIteratorT = typename ContainerT::const_iterator;

		Grid() :
			mWidth( 1 )
			, mHeight( 1 )
			, mContainer( 1 * 1 )
			, mIndexConverter( 1, 1 )
		{}

	public:
		std::size_t GetWidth() const { return mWidth; }
		std::size_t GetHeight() const { return mHeight; }
		bool IsIn( const std::size_t x, const std::size_t y ) const
		{
			return ( GetWidth() > x && GetHeight() > y );
		}

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

		//
		// Getter
		//
		CellT& Get( const std::size_t linear_idx )
		{
			if( linear_idx >= static_cast<int>( mContainer.size() ) )
			{
				static CellT dummy;
				return dummy;
			}

			return mContainer[linear_idx];
		}
		CellT& Get( const std::size_t x, const std::size_t y )
		{
			return Get( mIndexConverter.To_Linear( x, y ) );
		}
		const CellT& Get( const std::size_t linear_idx ) const
		{
			if( linear_idx >= static_cast<int>( mContainer.size() ) )
			{
				static CellT dummy;
				return dummy;
			}

			return mContainer[linear_idx];
		}
		const CellT& Get( const std::size_t x, const std::size_t y ) const
		{
			return Get( mIndexConverter.To_Linear( x, y ) );
		}

		//
		// Setter
		//
		void Set( const std::size_t linear_idx, const CellT& new_value )
		{
			if( linear_idx >= static_cast<int>( mContainer.size() ) )
			{
				return;
			}

			mContainer[linear_idx] = new_value;
		}
		void Set( const std::size_t x, const std::size_t y, const CellT& new_value )
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