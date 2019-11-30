#pragma once

#include <vector>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		using AnalyzerSp = std::shared_ptr<class Analyzer>;

		class Delegator : public cocos2d::Node
		{
		private:
			Delegator();

		public:
			static Delegator* create();

			void onEnter() override;
			void update( float _dt ) override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );

		public:
			void addAnalyzer( AnalyzerSp& _new_analyzer );

		private:
			cocos2d::EventListenerKeyboard* keyboard_listener;
			AnalyzerSp analyzer;
		};
	}
}