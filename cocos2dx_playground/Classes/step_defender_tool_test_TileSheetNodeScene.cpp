#include "step_defender_tool_test_TileSheetNodeScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCTextureCache.h"

#include "step_defender_tool_TileSheetNode.h"

USING_NS_CC;

namespace
{
	const int TAG_SelectedTileView = 100001;
}

namespace step_defender
{
	namespace tool_test
	{
		TileSheetNodeScene::TileSheetNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mTileSheetNode( nullptr )
		{}

		Scene* TileSheetNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) TileSheetNodeScene( back_to_the_previous_scene_callback );
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

		bool TileSheetNodeScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleOrigin = _director->getVisibleOrigin();
			const auto visibleSize = _director->getVisibleSize();
			const Vec2 visibleCenter( visibleOrigin.x + ( visibleSize.width * 0.5f ), visibleOrigin.y + ( visibleSize.height * 0.5f ) );

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";

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
				auto background_layer = LayerColor::create( Color4B( 7, 39, 43, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Tile Sheet Node
			//
			{
				mTileSheetNode = step_defender::tool::TileSheetNode::create(
					step_defender::tool::TileSheetNode::Config{
						32 , 32
						, "textures/texture_001.png"
					}
				);
				mTileSheetNode->setPosition(
					Vec2( visibleOrigin.x + ( visibleSize.width * 0.7f ), visibleCenter.y )
					- Vec2( mTileSheetNode->getContentSize().width * 0.5f, mTileSheetNode->getContentSize().height * 0.5f )
				);
				mTileSheetNode->SetSelectCallback( CC_CALLBACK_2( TileSheetNodeScene::onTileSelect, this ) );
				addChild( mTileSheetNode );
			}

			//
			// Selected Tile View
			//
			{
				auto texture = _director->getTextureCache()->getTextureForKey( "textures/texture_001.png" );
				
				auto sprite = Sprite::createWithTexture( texture );
				sprite->setTag( TAG_SelectedTileView );
				sprite->setScale( _director->getContentScaleFactor() );
				sprite->setPosition(
					Vec2( visibleOrigin.x + ( visibleSize.width * 0.3f ) ,visibleCenter.y )
				);
				addChild( sprite );
				onTileSelect( 0, 0 );

				// Guide
				auto guide = LayerColor::create( Color4B( 0u, 0u, 0u, 60u ), sprite->getBoundingBox().size.width, sprite->getBoundingBox().size.height );
				guide->setPosition( sprite->getPosition() - Vec2( guide->getContentSize().width * 0.5f, guide->getContentSize().height * 0.5f ) );
				addChild( guide, -1 );
			}

			return true;
		}

		void TileSheetNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TileSheetNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void TileSheetNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void TileSheetNodeScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}

		void TileSheetNodeScene::onTileSelect( const int x, const int y )
		{
			CCLOG( "%d, %d", x, y );

			auto sprite = static_cast<Sprite*>( getChildByTag( TAG_SelectedTileView ) );
			sprite->setTextureRect( mTileSheetNode->ConvertTilePoint2Rect( x, y ) );
		}
	}
}
