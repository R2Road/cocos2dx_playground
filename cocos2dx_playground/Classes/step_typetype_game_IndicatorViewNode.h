#pragma once

#include "2d/CCNode.h"

namespace step_typetype
{
	namespace game
	{
		class IndicatorViewNode : public cocos2d::Node
		{
		public:
			struct Config
			{
				bool bShowPivot = false;
				bool bShowBackgroundGuide = false;
			};

		private:
			IndicatorViewNode();

		public:
			static IndicatorViewNode* create( const Config config );

		private:
			bool init( const Config config );

		public:
			void Reset( const std::size_t current_stage_length );
			void SetIndicatorPosition( const std::size_t target_pos );

		private:
			cocos2d::Node* mIndicator;
		};
	}
}
