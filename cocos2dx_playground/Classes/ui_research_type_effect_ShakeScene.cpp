#include "ui_research_type_effect_ShakeScene.h"

#include <algorithm>
#include <new>
#include <numeric>
#include <random>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventListenerMouse.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

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
		ShakeScene::ShakeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mTypeDelay( 0.05f )
			, mElapsedTime( 0.f )
			, mLetterIndicator( 0 )
		{}

		Scene* ShakeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) ShakeScene( back_to_the_previous_scene_callback );
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

		bool ShakeScene::init()
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
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[SPACE] : Start Type";
				ss << cpg::linefeed;
				ss << "[A] : End Type";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Arrow Up] : Delay - Increase";
				ss << cpg::linefeed;
				ss << "[Arrow Uown] : Delay - Decrease";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 14 );
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
				const std::u32string u32_string = U"ㄱㄴㄷㄹㅁㅂㅅㅇㅈㅊㅋㅌㅍㅎ\n아야어여오 요우유으이\n가나다라마 바사아자차 카타파하 1234567890\n화려한 조명이 나를 감싸네";
				std::string utf8_string;
				StringUtils::UTF32ToUTF8( u32_string, utf8_string );

				auto label = Label::createWithTTF( utf8_string, cpg::StringTable::GetFontPath(), 15, Size::ZERO, TextHAlignment::CENTER );
				label->setTag( TAG_TextView );
				label->setTextColor( Color4B::YELLOW );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( label, std::numeric_limits<int>::max() );

				// build letters and setup action
				{
					std::mt19937 random_engine{ std::random_device{}( ) };
					std::uniform_real_distribution<> dist_time( 0.03f, 0.1f );
					std::uniform_real_distribution<> dist_move( 1.f, 2.f );
					Action* shake_action = nullptr;

					for( int i = 0; label->getStringLength() >= i; ++i )
					{
						// build letter
						auto letter = label->getLetter( i );
						if( !letter )
						{
							continue;
						}

						if( i % 10 < 5 )
						{
							continue;
						}

						// build action
						auto move_x = dist_move( random_engine );
						auto move_by_left = MoveBy::create( dist_time( random_engine ), Vec2( -move_x, 0.f ) );
						auto move_by_right = MoveBy::create( dist_time( random_engine ), Vec2( move_x, 0.f ) );
						auto sequence_action_move_horizontal = Sequence::create( move_by_left, move_by_right, nullptr );

						auto move_y = dist_move( random_engine );
						auto move_by_down = MoveBy::create( dist_time( random_engine ), Vec2( 0.f, -move_y ) );
						auto move_by_up = MoveBy::create( dist_time( random_engine ), Vec2( 0.f, move_y ) );
						auto sequence_action_move_vertical = Sequence::create( move_by_down, move_by_up, nullptr );

						shake_action = RepeatForever::create( Spawn::create( 
							sequence_action_move_horizontal
							, sequence_action_move_vertical
							, nullptr
						) );

						letter->runAction( shake_action );
					}
				}
			}

			return true;
		}

		void ShakeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ShakeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ShakeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void ShakeScene::HideLetters()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_TextView ) );
			for( auto letter : label->getChildren() )
			{
				letter->setVisible( false );
			}
		}
		void ShakeScene::StartType()
		{
			if( isScheduled( SEL_SCHEDULE( &ShakeScene::updateForType ) ) )
			{
				EndType();
			}

			HideLetters();
			schedule( SEL_SCHEDULE( &ShakeScene::updateForType ) );
		}
		void ShakeScene::EndType()
		{
			unscheduleAllCallbacks();

			mElapsedTime = 0.f;
			mLetterIndicator = 0;
		}
		void ShakeScene::updateForType( float dt )
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

		void ShakeScene::updateDelayView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_DelayView ) );
			label->setString( StringUtils::format( "Delay : %.2f", mTypeDelay ) );
		}

		void ShakeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
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
