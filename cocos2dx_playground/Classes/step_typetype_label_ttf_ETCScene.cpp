#include "step_typetype_label_ttf_ETCScene.h"

#include <new>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"

USING_NS_CC;

namespace step_typetype
{
	namespace label_ttf
	{
		ETCScene::ETCScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* ETCScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) ETCScene( back_to_the_previous_scene_callback );
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

		bool ETCScene::init()
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

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 5, 29, 81, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}
			
			//
			// Dimensions
			//
			{
				const char* label_string = "Dimensions( 20, 200 ) \n ----------------------------- \n";

				auto label = Label::createWithTTF( label_string, "fonts/NanumSquareR.ttf", 9, Size( 20, 200 ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.3f, visibleSize.height * 0.5f )
				);
				addChild( label );
			}

			//
			// Overflow
			//
			{
				// Clamp
				{
					const char* label_string = "Overflow : Clamp \n ----------------------------- \n Dimensions( 70, 20 )";

					auto label = Label::createWithTTF( label_string, "fonts/NanumSquareR.ttf", 9, Size( 70, 20 ) );
					label->setColor( Color3B::YELLOW );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.6f, visibleSize.height * 0.75f )
					);
					label->setOverflow( Label::Overflow::CLAMP );
					addChild( label );
				}

				// Shrink
				{
					const char* label_string = "Overflow : Shrink \n ----------------------------- \n Dimensions( 70, 20 )";

					auto label = Label::createWithTTF( label_string, "fonts/NanumSquareR.ttf", 9, Size( 70, 20 ) );
					label->setColor( Color3B::YELLOW );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.6f, visibleSize.height * 0.5f )
					);
					label->setOverflow( Label::Overflow::SHRINK );
					addChild( label );
				}

				// Resize Height
				{
					const char* label_string = "Overflow : Resize Height \n ----------------------------- \n Dimensions( 70, 20 )";

					auto label = Label::createWithTTF( label_string, "fonts/NanumSquareR.ttf", 9, Size( 70, 20 ) );
					label->setColor( Color3B::YELLOW );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.6f, visibleSize.height * 0.25f )
					);
					label->setOverflow( Label::Overflow::RESIZE_HEIGHT );
					addChild( label );
				}
			}

			return true;
		}

		void ETCScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ETCScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ETCScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void ETCScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
