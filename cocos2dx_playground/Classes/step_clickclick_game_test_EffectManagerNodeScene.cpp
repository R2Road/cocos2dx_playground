#include "step_clickclick_game_test_EffectManagerNodeScene.h"

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

#include "step_clickclick_game_EffectManagerNode.h"

USING_NS_CC;

namespace
{
	const int EFFECT_MANAGER_WIDTH = 7;
	const int EFFECT_MANAGER_HEIGHT = 6;
}

namespace step_clickclick
{
	namespace game_test
	{
		EffectManagerNodeScene::EffectManagerNodeScene() :
			mKeyboardListener( nullptr )

			, mEffectManagerNode( nullptr )
			, mGridIndexConverter( EFFECT_MANAGER_WIDTH, EFFECT_MANAGER_HEIGHT )
		{}

		Scene* EffectManagerNodeScene::create()
		{
			auto ret = new ( std::nothrow ) EffectManagerNodeScene();
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

		bool EffectManagerNodeScene::init()
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
				mEffectManagerNode = game::EffectManagerNode::create( game::EffectManagerNode::Config{ true }, EFFECT_MANAGER_WIDTH, EFFECT_MANAGER_HEIGHT );
				mEffectManagerNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mEffectManagerNode );
			}

			return true;
		}

		void EffectManagerNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( EffectManagerNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void EffectManagerNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void EffectManagerNodeScene::PlayAllEffect( const game::eEffectIndex effect_index )
		{
			for( int y = 0; EFFECT_MANAGER_HEIGHT > y; ++y )
			{
				for( int x = 0; EFFECT_MANAGER_WIDTH > x; ++x )
				{
					mEffectManagerNode->PlayEffect( mGridIndexConverter.To_Linear( x, y ), effect_index );
				}
			}
		}


		void EffectManagerNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_clickclick::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_Q:
				PlayAllEffect( game::eEffectIndex::Increase );
				break;
			case EventKeyboard::KeyCode::KEY_W:
				PlayAllEffect( game::eEffectIndex::Decrease );
				break;
			case EventKeyboard::KeyCode::KEY_E:
				PlayAllEffect( game::eEffectIndex::Die );
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
