#pragma once

#include <vector>

#include "2d/CCNode.h"

namespace step_typetype
{
	namespace game
	{
		class LetterViewNode;
		class Stage;

		class StageViewNode : public cocos2d::Node
		{
		public:
			struct Config
			{
				bool bShowPivot = false;
			};

		private:
			using ContainerT = std::vector<LetterViewNode*>;

			StageViewNode( const std::size_t max_length );

		public:
			static StageViewNode* create( const std::size_t max_length, const Config config );

		private:
			bool init( const Config config );

		public:
			void Reset( const Stage& stage );
			void RequestLetterDie( const std::size_t target_pos );

		private:
			ContainerT mLetters;
		};
	}
}
