#include "step_typetype_label_ttf_AlignScene.h"

#include <new>

#include "cocos2d.h"

#include "cpg_SStream.h"

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace step_typetype
{
	namespace label_ttf
	{
		AlignScene::AlignScene() : mKeyboardListener( nullptr ) {}

		Scene* AlignScene::create()
		{
			auto ret = new ( std::nothrow ) AlignScene();
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

		bool AlignScene::init()
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
				auto background_layer = LayerColor::create( Color4B( 79, 10, 5, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Align Test
			//
			{
				// Label 1
				{
					const char* label_string = "Test Label \n ----------------------------- \n Align Center";

					auto label = Label::createWithTTF( label_string, "fonts/NanumSquareR.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.25f )
					);
					addChild( label );
				}

				// Label 2
				{
					const char* label_string = "Test Label \n ----------------------------- \n Align Left";

					auto label = Label::createWithTTF( label_string, "fonts/NanumSquareR.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
					label->setColor( Color3B( 0, 186, 255 ) );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
					);
					addChild( label );
				}

				// Label 3
				{
					const char* label_string = "Test Label \n ----------------------------- \n Align Right";

					auto label = Label::createWithTTF( label_string, "fonts/NanumSquareR.ttf", 9, Size::ZERO, TextHAlignment::RIGHT );
					label->setColor( Color3B( 255, 132, 175 ) );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.75f )
					);
					addChild( label );
				}
			}

			return true;
		}

		void AlignScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( AlignScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void AlignScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void AlignScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_typetype::RootScene::create() );
				return;
			}
		}
	}
}
