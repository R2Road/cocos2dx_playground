#pragma once

#include "2d/CCNode.h"

NS_CC_BEGIN
	class Action;
	class Label;
	class Layer;
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
			~MessageViewNode();

			static MessageViewNode* create();

		private:
			bool init() override;

		public:
			bool isMessaging() const;
			void ShowMessage( const char* str );

		private:
			cocos2d::Layer* mLayer;
			cocos2d::Label* mLabel;
			cocos2d::Action* mFadeInOutAction;
		};
	}
}
