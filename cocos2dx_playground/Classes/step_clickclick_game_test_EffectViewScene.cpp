#include "step_clickclick_game_test_EffectViewScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_StringTable.h"

#include "step_clickclick_RootScene.h"

#include "step_clickclick_game_EffectView.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game_test
	{
		EffectViewScene::EffectViewScene() :
			mKeyboardListener( nullptr )
			, mEffectView( nullptr )
		{}

		Scene* EffectViewScene::create()
		{
			auto ret = new ( std::nothrow ) EffectViewScene();
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

		bool EffectViewScene::init()
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
				ss << "[1] : Run Increase Effect";
				ss << std::endl;
				ss << "[2] : Run Decrease Effect";
				ss << std::endl;
				ss << "[3] : Run Die Effect";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::WHITE );
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
				auto background_layer = LayerColor::create( Color4B( 0, 61, 33, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Effect View
			//
			{
				mEffectView = game::EffectView::create();
				mEffectView->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( mEffectView );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( _director->getContentScaleFactor() );
					mEffectView->addChild( pivot, std::numeric_limits<int>::min() );
				}
			}

			return true;
		}

		void EffectViewScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( EffectViewScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void EffectViewScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void EffectViewScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_clickclick::RootScene::create() );
				break;

			case EventKeyboard::KeyCode::KEY_1:
				mEffectView->PlayEffect( game::EffectView::eAnimationIndex::Increase );
				break;
			case EventKeyboard::KeyCode::KEY_2:
				mEffectView->PlayEffect( game::EffectView::eAnimationIndex::Decrease );
				break;
			case EventKeyboard::KeyCode::KEY_3:
				mEffectView->PlayEffect( game::EffectView::eAnimationIndex::Die );
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
