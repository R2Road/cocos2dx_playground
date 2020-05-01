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

		, mLabel_forUpdate( nullptr )
		, mLabel_forCustomeUpdate( nullptr )
		, mLabel_forLambda( nullptr )

		, mElapsedTime_forUpdate( 0.f )
		, mElapsedTime_forCustomeUpdate( 0.f )
		, mElapsedTime_forLambda( 0.f )
	{}

	Scene* ScheduleScene::create()
	{
		auto ret = new ( std::nothrow ) ScheduleScene();
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
			return nullptr;
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

		const auto visibleSize = Director::getInstance()->getVisibleSize();
		const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();


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
			ss << "[3] : Unschedule Lambda";

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition( Vec2(
				visibleOrigin.x
				, visibleOrigin.y + visibleSize.height
			) );
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
		// Label
		//
		{
			// Update
			mLabel_forUpdate = Label::createWithTTF( "0.00", "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
			mLabel_forUpdate->setColor( Color3B::GREEN );
			mLabel_forUpdate->setPosition(
				visibleOrigin.x + ( visibleSize.width * 0.3f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			);
			addChild( mLabel_forUpdate );

			// Custome Update
			mLabel_forCustomeUpdate = Label::createWithTTF( "0.00", "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
			mLabel_forCustomeUpdate->setColor( Color3B::GREEN );
			mLabel_forCustomeUpdate->setPosition(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			);
			addChild( mLabel_forCustomeUpdate );

			// Lambda Update
			mLabel_forLambda = Label::createWithTTF( "0.00", "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
			mLabel_forLambda->setColor( Color3B::GREEN );
			mLabel_forLambda->setPosition(
				visibleOrigin.x + ( visibleSize.width * 0.7f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			);
			addChild( mLabel_forLambda );
		}

		//
		// Scuedule
		//
		{
			// Call - void update( float dt ) override;
			scheduleUpdate();

			// Call - void CustomeUpdate( float dt );
			schedule( schedule_selector( ScheduleScene::CustomeUpdate ), 0.5f );

			// Call - Lambda
			schedule(
				[this]( float dt )
				{
					mElapsedTime_forLambda += dt;
					if( 10.f < mElapsedTime_forLambda )
					{
						mElapsedTime_forLambda = 0.f;
					}
					mLabel_forLambda->setString( StringUtils::format( "%.2f", mElapsedTime_forLambda ) );
				}
				, 1.f
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

		Node::onExit();
	}
	void ScheduleScene::update( float dt )
	{
		mElapsedTime_forUpdate += dt;
		if( 10.f < mElapsedTime_forUpdate )
		{
			mElapsedTime_forUpdate = 0.f;
		}
		mLabel_forUpdate->setString( StringUtils::format( "%.2f", mElapsedTime_forUpdate ) );

		Node::update( dt );
	}
	void ScheduleScene::CustomeUpdate( float dt )
	{
		mElapsedTime_forCustomeUpdate += dt;
		if( 10.f < mElapsedTime_forCustomeUpdate )
		{
			mElapsedTime_forCustomeUpdate = 0.f;
		}
		mLabel_forCustomeUpdate->setString( StringUtils::format( "%.2f", mElapsedTime_forCustomeUpdate ) );
	}

	void ScheduleScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
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
			if( isScheduled( schedule_selector( ScheduleScene::CustomeUpdate ) ) )
			{
				unschedule( schedule_selector( ScheduleScene::CustomeUpdate ) );
			}
		}
		if( EventKeyboard::KeyCode::KEY_3 == keycode )
		{
			if( isScheduled( "Update for Lambda" ) )
			{
				unschedule( "Update for Lambda" );
			}
		}

		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			Director::getInstance()->replaceScene( step_typetype::RootScene::create() );
			return;
		}
	}
}
