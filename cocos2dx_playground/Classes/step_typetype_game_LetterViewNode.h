#pragma once

#include "2d/CCNode.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_typetype
{
	namespace game
	{
		class LetterViewNode : public cocos2d::Node
		{
		private:
			LetterViewNode();

		public:
			static LetterViewNode* create();

		private:
			bool init() override;

		public:
			void Reset( const char letter );
			void Die();

		private:
			cocos2d::Label* mLabel;
		};
	}
}
