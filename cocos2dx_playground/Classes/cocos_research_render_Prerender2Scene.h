#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
	class RenderTexture;
NS_CC_END

namespace cocos_research_render
{
	class Prerender2Scene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		Prerender2Scene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Pre-Render 2"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void requestCapture();
		void test_UpdateEnd( float dt );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		cocos2d::Node* mActorNode;
		cocos2d::Node* mCaptureTargetNode;
		cocos2d::RenderTexture* mRenderTextureNode;

		bool mbInputBlock;
		cocos2d::Vec2 mCapturePivot;
	};
}
