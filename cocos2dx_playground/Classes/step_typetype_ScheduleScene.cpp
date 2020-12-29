#include "step_typetype_ScheduleScene.h"

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

namespace
{
	const char* STRING_Name_LambdaSchedule = "Update for Lambda";
}

namespace step_typetype
{
	class ScheduleView : public Node
	{
	private:
		ScheduleView() : mViewLabel( nullptr ), mElapsedtime( 0.f ) {}
	public:
		static ScheduleView* create( const char* view_name )
		{
			auto ret = new ( std::nothrow ) ScheduleView();
			if( !ret || !ret->init( view_name ) )
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

	private:
		bool init( const char* view_name )
		{
			if( !Node::init() )
			{
				return false;
			}

			setName( view_name );

			mViewLabel = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::CENTER );
			mViewLabel->setColor( Color3B::GREEN );
			addChild( mViewLabel );

			UpdateSchedule( 0.f );

			return true;
		}

	public:
		void UpdateSchedule( float dt )
		{
			mElapsedtime += dt;
			if( 10.f < mElapsedtime )
			{
				mElapsedtime = 0.f;
			}
			
			mViewLabel->setString( StringUtils::format( "%s\n%.2f", getName().c_str(), mElapsedtime ) );
		}

	private:
		Label* mViewLabel;
		float mElapsedtime;
	};

	ScheduleScene::ScheduleScene() :
		mKeyboardListener( nullptr )

		, mScheduleView4Update()
		, mScheduleView4CustomeUpdate()
		, mScheduleView4Lambda()
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
		// Schedule View - Update
		//
		{
			mScheduleView4Update = ScheduleView::create( "Update" );
			mScheduleView4Update->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.25f, visibleSize.height * 0.5f )
			);
			addChild( mScheduleView4Update );
		}

		//
		// Schedule View - Custome Update
		//
		{
			mScheduleView4CustomeUpdate = ScheduleView::create( "Custome Update" );
			mScheduleView4CustomeUpdate->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
			);
			addChild( mScheduleView4CustomeUpdate );
		}

		//
		// Schedule View - Lambda Update
		//
		{
			mScheduleView4Lambda = ScheduleView::create( "Lambda Update" );
			mScheduleView4Lambda->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.75f, visibleSize.height * 0.5f )
			);
			addChild( mScheduleView4Lambda );
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
					mScheduleView4Lambda->UpdateSchedule( dt );
				}
				, 0.5f
				, STRING_Name_LambdaSchedule
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
		mScheduleView4Update->UpdateSchedule( dt );

		Node::update( dt );
	}
	void ScheduleScene::customeUpdate( float dt )
	{
		mScheduleView4CustomeUpdate->UpdateSchedule( dt );
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
			if( isScheduled( STRING_Name_LambdaSchedule ) )
			{
				unschedule( STRING_Name_LambdaSchedule );
			}
		}
	}
}
