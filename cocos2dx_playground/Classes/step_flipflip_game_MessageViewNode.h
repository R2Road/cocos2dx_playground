#pragma once

#include "2d/CCNode.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_flipflip
{
	namespace game
	{
		class MessageViewNode : public cocos2d::Node
		{
		private:
			MessageViewNode();

		public:
			static MessageViewNode* create();

		private:
			bool init() override;

		public:
			void ShowMessage( const char* str );

		private:
			cocos2d::Label* mLabel;
		};
	}
}
