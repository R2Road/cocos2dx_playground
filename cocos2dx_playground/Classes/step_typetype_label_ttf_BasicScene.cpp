#include "step_typetype_label_ttf_BasicScene.h"

#include <new>

#include "cpg_StringTable.h"
#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace step_typetype
{
	namespace label_ttf
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr ) {}

		Scene* BasicScene::create()
		{
			auto ret = new ( std::nothrow ) BasicScene();
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

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( 0.f, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Align Test
			//
			{
				// Label 1
				{
					const char* label_string = "Test Label \n ----------------------------- \n Align Center";

					auto label = Label::createWithTTF( label_string, cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::CENTER );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.25f, visibleSize.height * 0.25f )
					);
					addChild( label );
				}

				// Label 2
				{
					const char* label_string = "Test Label \n ----------------------------- \n Align Left";

					auto label = Label::createWithTTF( label_string, cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
					label->setColor( Color3B::BLUE );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.25f, visibleSize.height * 0.5f )
					);
					addChild( label );
				}

				// Label 3
				{
					const char* label_string = "Test Label \n ----------------------------- \n Align Right";

					auto label = Label::createWithTTF( label_string, cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::RIGHT );
					label->setColor( Color3B::RED );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.25f, visibleSize.height * 0.75f )
					);
					addChild( label );
				}
			}

			//
			// Dimensions
			//
			{
				const char* label_string = "Dimensions( 20, 200 ) \n ----------------------------- \n";

				auto label = Label::createWithTTF( label_string, cpg::StringTable::GetFontPath(), 9, Size( 20, 200 ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
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

					auto label = Label::createWithTTF( label_string, cpg::StringTable::GetFontPath(), 9, Size( 70, 20 ) );
					label->setColor( Color3B::YELLOW );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.75f, visibleSize.height * 0.75f )
					);
					label->setOverflow( Label::Overflow::CLAMP );
					addChild( label );
				}

				// Shrink
				{
					const char* label_string = "Overflow : Shrink \n ----------------------------- \n Dimensions( 70, 20 )";

					auto label = Label::createWithTTF( label_string, cpg::StringTable::GetFontPath(), 9, Size( 70, 20 ) );
					label->setColor( Color3B::YELLOW );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.75f, visibleSize.height * 0.5f )
					);
					label->setOverflow( Label::Overflow::SHRINK );
					addChild( label );
				}

				// Resize Height
				{
					const char* label_string = "Overflow : Resize Height \n ----------------------------- \n Dimensions( 70, 20 )";

					auto label = Label::createWithTTF( label_string, cpg::StringTable::GetFontPath(), 9, Size( 70, 20 ) );
					label->setColor( Color3B::YELLOW );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.75f, visibleSize.height * 0.25f )
					);
					label->setOverflow( Label::Overflow::RESIZE_HEIGHT );
					addChild( label );
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

		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_typetype::RootScene::create() );
				return;
			}
		}
	}
}
