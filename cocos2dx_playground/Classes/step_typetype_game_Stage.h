#pragma once

#include <vector>

namespace step_typetype
{
	namespace game
	{
		class Stage
		{
		private:
			using ContainerT = std::vector<char>;

		public:
			Stage( const std::size_t max_length );

			void Reset( const std::size_t length );

			std::size_t GetLength() const { return mLength; }
			std::size_t GetLength_MAX() const { return mLetters.capacity(); }
			std::size_t GetIndicator_Current() const { return mIndicator_Current; }
			std::size_t GetIndicator_End() const { return mIndicator_End; }
			char GetLetter( const std::size_t target_pos ) const;
			bool RequestLetterDie( const char letter );

			bool IsStageClear() const;

		private:
			std::size_t mLength;
			ContainerT mLetters;

			std::size_t mIndicator_Current;
			std::size_t mIndicator_End;
		};
	}
}
