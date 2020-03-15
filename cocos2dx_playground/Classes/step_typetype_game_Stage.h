#pragma once

#include <vector>

namespace step_typetype
{
	namespace game
	{
		class Stage
		{
		public:
			using ContainerT = std::vector<char>;

			Stage( const std::size_t max_length );

			void Reset( const std::size_t length );

			std::size_t GetIndicator_Current() const { return mIndicator_Current; }
			std::size_t GetIndicator_End() const { return mIndicator_End; }
			char GetLetter( const std::size_t target_pos ) const;
			bool RequestRemoveLetter( const char letter );

		private:
			const char mLetter_Min;
			const char mLetter_Max;
			ContainerT mLetters;

			std::size_t mIndicator_Current;
			std::size_t mIndicator_End;
		};
	}
}
