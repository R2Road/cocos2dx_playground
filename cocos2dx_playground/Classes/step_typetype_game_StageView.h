#pragma once

#include <vector>

#include "2d/CCNode.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_typetype
{
	namespace game
	{
		class Stage;

		struct StageViewConfig
		{
			bool bShowPivot = false;
			bool bShowBackgroundGuide = false;
		};

		class StageView : public cocos2d::Node
		{
		private:
			using ContainerT = std::vector<cocos2d::Label*>;

			StageView( const std::size_t max_length );

		public:
			static StageView* create( const std::size_t max_length, const StageViewConfig config = StageViewConfig() );

			bool init( const StageViewConfig config );

			void Reset( const Stage& stage );

		private:
			ContainerT mLetters;
		};
	}
}
