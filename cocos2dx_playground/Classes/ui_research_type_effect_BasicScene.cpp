#include "ui_research_type_effect_BasicScene.h"

#include <algorithm>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventListenerMouse.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

USING_NS_CC;

namespace
{
	const int TAG_TextView = 20140416;
	const int TAG_DelayView = 20160528;
}

namespace ui_research
{
	namespace type_effect
	{
		BasicScene::BasicScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mTypeDelay( 0.05f )
			, mElapsedTime( 0.f )
			, mLetterIndicator( 0 )
		{}

		Scene* BasicScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) BasicScene( back_to_the_previous_scene_callback );
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

		bool BasicScene::init()
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
				ss << "[SPACE] : Start Type";
				ss << std::endl;
				ss << "[A] : End Type";
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow Up] : Delay - Increase";
				ss << std::endl;
				ss << "[Arrow Uown] : Delay - Decrease";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 50, 75, 112, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Delay View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 14, Size::ZERO, TextHAlignment::LEFT );
				label->setTag( TAG_DelayView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateDelayView();
			}

			//
			// Research
			//
			{
				auto label = Label::createWithTTF( "ABCDEF GHIJKL MNOPQ RSTUV\nWXYZ1 2345 67890", "fonts/NanumSquareR.ttf", 14, Size::ZERO, TextHAlignment::CENTER );
				label->setTag( TAG_TextView );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( label, std::numeric_limits<int>::max() );

				// build letters
				for( int i = 0; label->getStringLength() > i; ++i )
				{
					label->getLetter( i );
				}
			}

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void BasicScene::HideLetters()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_TextView ) );
			for( auto letter : label->getChildren() )
			{
				letter->setVisible( false );
			}
		}
		void BasicScene::StartType()
		{
			if( isScheduled( SEL_SCHEDULE( &BasicScene::updateForType ) ) )
			{
				EndType();
			}

			HideLetters();
			schedule( SEL_SCHEDULE( &BasicScene::updateForType ) );
		}
		void BasicScene::EndType()
		{
			unscheduleAllCallbacks();

			mElapsedTime = 0.f;
			mLetterIndicator = 0;
		}
		void BasicScene::updateForType( float dt )
		{
			mElapsedTime += dt;
			if( mTypeDelay > mElapsedTime )
			{
				return;
			}

			mElapsedTime = 0.f;

			auto label = static_cast<Label*>( getChildByTag( TAG_TextView ) );
			auto letter = label->getLetter( mLetterIndicator );
			if( letter )
			{
				letter->setVisible( true );
			}

			++mLetterIndicator;
			if( label->getStringLength() <= mLetterIndicator )
			{
				EndType();
			}
		}

		void BasicScene::updateDelayView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_DelayView ) );
			label->setString( StringUtils::format( "Delay : %.2f", mTypeDelay ) );
		}

		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_SPACE:
				StartType();
				break;
			case EventKeyboard::KeyCode::KEY_A:
				EndType();
				break;

			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mTypeDelay += 0.01f;
				updateDelayView();
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mTypeDelay = std::max( 0.f, mTypeDelay - 0.01f );
				updateDelayView();
				break;
			}
		}
	}
}
