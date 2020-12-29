#include "step_typetype_ScheduleScene.h"

#include <algorithm>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace step_typetype
{
	ScheduleScene::ScheduleScene() :
		mKeyboardListener( nullptr )

		, mScheduleView_forUpdate()
		, mScheduleView_forCustomeUpdate()
		, mScheduleView_forLambda()
	{}

	Scene* ScheduleScene::create()
	{
		auto ret = new ( std::nothrow ) ScheduleScene();
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
		}
		else
		{
			ret->autorelease();
		}

		return ret;
	}

	bool ScheduleScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleSize = _director->getVisibleSize();
		const auto visibleOrigin = _director->getVisibleOrigin();


		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[ESC] : Return to Root";
			ss << std::endl;
			ss << std::endl;
			ss << "[F1] : Unschedule All";
			ss << std::endl;
			ss << std::endl;
			ss << "[1] : Unschedule Update";
			ss << std::endl;
			ss << "[2] : Unschedule Custome Update";
			ss << std::endl;
			ss << "[3] : Unschedule Lambda Update";

			auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition(
				visibleOrigin
				+ Vec2( 0.f, visibleSize.height )
			);
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 79, 10, 5, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// Label - Update
		//
		{
			mScheduleView_forUpdate.ViewLabel = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::CENTER );
			mScheduleView_forUpdate.ViewLabel->setName( "Update" );
			mScheduleView_forUpdate.ViewLabel->setColor( Color3B::GREEN );
			mScheduleView_forUpdate.ViewLabel->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.25f, visibleSize.height * 0.5f )
			);
			addChild( mScheduleView_forUpdate.ViewLabel );

			updateLabel( mScheduleView_forUpdate);
		}

		//
		// Label - Custome Update
		//
		{
			mScheduleView_forCustomeUpdate.ViewLabel = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::CENTER );
			mScheduleView_forCustomeUpdate.ViewLabel->setName( "Custome Update" );
			mScheduleView_forCustomeUpdate.ViewLabel->setColor( Color3B::GREEN );
			mScheduleView_forCustomeUpdate.ViewLabel->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
			);
			addChild( mScheduleView_forCustomeUpdate.ViewLabel );

			updateLabel( mScheduleView_forCustomeUpdate );
		}

		//
		// Label - Lambda Update
		//
		{
			mScheduleView_forLambda.ViewLabel = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::CENTER );
			mScheduleView_forLambda.ViewLabel->setName( "Lambda Update" );
			mScheduleView_forLambda.ViewLabel->setColor( Color3B::GREEN );
			mScheduleView_forLambda.ViewLabel->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.75f, visibleSize.height * 0.5f )
			);
			addChild( mScheduleView_forLambda.ViewLabel );

			updateLabel( mScheduleView_forLambda );
		}

		//
		// Scuedule
		//
		{
			// Call - void update( float dt ) override;
			scheduleUpdate();

			// Call - void customeUpdate( float dt );
			schedule( schedule_selector( ScheduleScene::customeUpdate ), 0.1f );

			// Call - Lambda
			schedule(
				[this]( float dt )
				{
					mScheduleView_forLambda.Elapsedtime += dt;
					if( 10.f < mScheduleView_forLambda.Elapsedtime )
					{
						mScheduleView_forLambda.Elapsedtime = 0.f;
					}
					updateLabel( mScheduleView_forLambda );
				}
				, 0.5f
				, "Update for Lambda"
			);
		}

		return true;
	}

	void ScheduleScene::onEnter()
	{
		Scene::onEnter();
			
		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ScheduleScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void ScheduleScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}
	void ScheduleScene::update( float dt )
	{
		mScheduleView_forUpdate.Elapsedtime += dt;
		if( 10.f < mScheduleView_forUpdate.Elapsedtime )
		{
			mScheduleView_forUpdate.Elapsedtime = 0.f;
		}
		updateLabel( mScheduleView_forUpdate );

		Node::update( dt );
	}
	void ScheduleScene::customeUpdate( float dt )
	{
		mScheduleView_forCustomeUpdate.Elapsedtime += dt;
		if( 10.f < mScheduleView_forCustomeUpdate.Elapsedtime )
		{
			mScheduleView_forCustomeUpdate.Elapsedtime = 0.f;
		}
		updateLabel( mScheduleView_forCustomeUpdate );
	}

	void ScheduleScene::updateLabel( ScheduleView& schedule_view )
	{
		schedule_view.ViewLabel->setString( StringUtils::format( "%s\n%.2f", schedule_view.ViewLabel->getName().c_str(), schedule_view.Elapsedtime ) );
	}

	void ScheduleScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			_director->replaceScene( step_typetype::RootScene::create() );
			return;
		}

		if( EventKeyboard::KeyCode::KEY_F1 == keycode )
		{
			unscheduleAllCallbacks();
		}

		if( EventKeyboard::KeyCode::KEY_1 == keycode )
		{
			unscheduleUpdate();
		}
		if( EventKeyboard::KeyCode::KEY_2 == keycode )
		{
			if( isScheduled( schedule_selector( ScheduleScene::customeUpdate ) ) )
			{
				unschedule( schedule_selector( ScheduleScene::customeUpdate ) );
			}
		}
		if( EventKeyboard::KeyCode::KEY_3 == keycode )
		{
			if( isScheduled( "Update for Lambda" ) )
			{
				unschedule( "Update for Lambda" );
			}
		}
	}
}
