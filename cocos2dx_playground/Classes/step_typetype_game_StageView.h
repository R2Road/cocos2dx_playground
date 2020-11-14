#pragma once

#include <vector>

#include "2d/CCNode.h"

namespace step_typetype
{
	namespace game
	{
		class LetterView;
		class Stage;

		struct StageViewConfig
		{
			bool bShowPivot = false;
			bool bShowBackgroundGuide = false;
		};

		class StageView : public cocos2d::Node
		{
		private:
			using ContainerT = std::vector<LetterView*>;

			StageView( const std::size_t max_length );

		public:
			static StageView* create( const std::size_t max_length, const StageViewConfig config = StageViewConfig() );

		private:
			bool init( const StageViewConfig config );

		public:
			void Reset( const Stage& stage );
			void RequestLetterDie( const std::size_t target_pos );

		private:
			ContainerT mLetters;
			cocos2d::Node* mIndicator;
		};
	}
}
