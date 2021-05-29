#include "step_rain_of_chaos_game_test_BackgroundNodeScene.h"

#include <array>
#include <new>
#include <numeric>
#include <utility>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_rain_of_chaos_game_BackgroundNode.h"

USING_NS_CC;

namespace
{
	const int TAG_BackgroundNode = 20140416;
	const int TAG_AmountView = 20160528;
}

namespace step_rain_of_chaos
{
	namespace game_test
	{
		BackgroundNodeScene::BackgroundNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mHorizontalAmount( 5 )
			, mVerticalAmount( 5 )
		{}

		Scene* BackgroundNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) BackgroundNodeScene( back_to_the_previous_scene_callback );
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

		bool BackgroundNodeScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleOrigin = _director->getVisibleOrigin();
			const auto visibleSize = _director->getVisibleSize();

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
				ss << "[A] : Reset";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Arrow U/D] : Change Vertical Size";
				ss << cpg::linefeed;
				ss << "[Arrow L/R] : Change Horizontal Size";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 63, 23, 14, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Amount View
			//
			{
				auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 14, Size::ZERO, TextHAlignment::RIGHT );
				label->setTag( TAG_AmountView );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateAmountView();
			}

			//
			// Background Node
			//
			{
				std::vector<SpriteFrame*> SpriteFrames{
					SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_0.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_1.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_2.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_3.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_4.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_5.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_6.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_7.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_8.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_9.png" )
				};

				auto background_node = step_rain_of_chaos::game::BackgroundNode::create( 10, 10, "textures/texture_001.png", std::move( SpriteFrames ) );
				background_node->setTag( TAG_BackgroundNode );
				background_node->setPosition(
					visibleOrigin.x + ( visibleSize.width * 0.5f ) - ( background_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f ) - ( background_node->getContentSize().height * 0.5f )
				);
				addChild( background_node );

				updateBackgroundNode();
			}

			return true;
		}

		void BackgroundNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BackgroundNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BackgroundNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void BackgroundNodeScene::updateAmountView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_AmountView ) );
			label->setString( StringUtils::format( "Horizontal Amount : %3d\nVertical Amount : %3d", mHorizontalAmount, mVerticalAmount ) );
		}
		void BackgroundNodeScene::updateBackgroundNode()
		{
			const auto visibleOrigin = _director->getVisibleOrigin();
			const auto visibleSize = _director->getVisibleSize();

			auto background_node = static_cast<step_rain_of_chaos::game::BackgroundNode*>( getChildByTag( TAG_BackgroundNode ) );
			background_node->Reset( mHorizontalAmount, mVerticalAmount );
			background_node->setPosition(
				visibleOrigin.x + ( visibleSize.width * 0.5f ) - ( background_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f ) - ( background_node->getContentSize().height * 0.5f )
			);
		}


		void BackgroundNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			if( EventKeyboard::KeyCode::KEY_A == keycode )
			{
				updateBackgroundNode();
			}

			if( EventKeyboard::KeyCode::KEY_UP_ARROW == keycode )
			{
				++mVerticalAmount;
				updateBackgroundNode();
				updateAmountView();
			}
			if( EventKeyboard::KeyCode::KEY_DOWN_ARROW == keycode )
			{
				mVerticalAmount = std::max( 1, mVerticalAmount - 1 );
				updateBackgroundNode();
				updateAmountView();
			}

			if( EventKeyboard::KeyCode::KEY_RIGHT_ARROW == keycode )
			{
				++mHorizontalAmount;
				updateBackgroundNode();
				updateAmountView();
			}
			if( EventKeyboard::KeyCode::KEY_LEFT_ARROW == keycode )
			{
				mHorizontalAmount = std::max( 1, mHorizontalAmount - 1 );
				updateBackgroundNode();
				updateAmountView();
			}
		}
	}
}
