#pragma once

#include "2d/CCNode.h"

NS_CC_BEGIN
	class Action;
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
			~MessageViewNode();

			static MessageViewNode* create( const cocos2d::Color4B& background_color );

		private:
			bool init( const cocos2d::Color4B& background_color );

		public:
			bool isMessaging() const;
			void ShowMessage( const char* str );

		private:
			cocos2d::Node* mLayer;
			cocos2d::Label* mLabel;
			cocos2d::Action* mFadeInOutAction;
		};
	}
}
