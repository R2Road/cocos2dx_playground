#pragma once

#include <vector>

#include "2d/CCNode.h"

namespace step_typetype
{
	namespace game
	{
		class LetterViewNode;
		class Stage;

		struct StageViewConfig
		{
			bool bShowPivot = false;
			bool bShowBackgroundGuide = false;
		};

		class StageViewNode : public cocos2d::Node
		{
		private:
			using ContainerT = std::vector<LetterViewNode*>;

			StageViewNode( const std::size_t max_length );

		public:
			static StageViewNode* create( const std::size_t max_length, const StageViewConfig config = StageViewConfig() );

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
