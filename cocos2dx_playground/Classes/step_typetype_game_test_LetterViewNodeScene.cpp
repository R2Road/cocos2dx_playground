#include "step_typetype_game_test_LetterViewNodeScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"

#include "step_typetype_game_LetterViewNode.h"

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace
{
	const char CHAR_first = 'A';
	const char CHAR_end = 'Z';
}

namespace step_typetype
{
	namespace game_test
	{
		LetterViewNodeScene::LetterViewNodeScene() :
			mKeyboardListener( nullptr )

			, mLetterViewNode( nullptr )
			, mCurrentCharacter( CHAR_first )
		{}

		Scene* LetterViewNodeScene::create()
		{
			auto ret = new ( std::nothrow ) LetterViewNodeScene();
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

		bool LetterViewNodeScene::init()
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
				ss << "[A] : Reset";
				ss << cpg::linefeed;
				ss << "[S] : Die";

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
			// Letter View
			//
			{
				mLetterViewNode = game::LetterViewNode::create();
				mLetterViewNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mLetterViewNode );
			}

			//
			// Setup
			//
			mLetterViewNode->Reset( mCurrentCharacter );

			return true;
		}

		void LetterViewNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( LetterViewNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void LetterViewNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void LetterViewNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_typetype::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_A:
			{
				++mCurrentCharacter;
				if( CHAR_end < mCurrentCharacter )
				{
					mCurrentCharacter = CHAR_first;
				}

				mLetterViewNode->Reset( mCurrentCharacter );
			}
			return;
			case EventKeyboard::KeyCode::KEY_S:
				mLetterViewNode->Die();
				return;
			}
		}
	}
}
