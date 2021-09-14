#include "step_clickclick_game_test_EffectViewScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_clickclick_RootScene.h"

#include "step_clickclick_game_EffectViewNode.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game_test
	{
		EffectViewScene::EffectViewScene() :
			mKeyboardListener( nullptr )
			, mEffectViewNode( nullptr )
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
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Q] : Run Effect - Increase";
				ss << cpg::linefeed;
				ss << "[W] : Run Effect - Decrease";
				ss << cpg::linefeed;
				ss << "[E] : Run Effect - Die";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::WHITE );
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
				auto background_layer = LayerColor::create( Color4B( 0, 61, 33, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Effect View
			//
			{
				mEffectViewNode = game::EffectViewNode::create();
				mEffectViewNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mEffectViewNode );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( _director->getContentScaleFactor() );
					mEffectViewNode->addChild( pivot, std::numeric_limits<int>::min() );
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

			case EventKeyboard::KeyCode::KEY_Q:
				mEffectViewNode->PlayEffect( game::eEffectIndex::Increase );
				break;
			case EventKeyboard::KeyCode::KEY_W:
				mEffectViewNode->PlayEffect( game::eEffectIndex::Decrease );
				break;
			case EventKeyboard::KeyCode::KEY_E:
				mEffectViewNode->PlayEffect( game::eEffectIndex::Die );
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
