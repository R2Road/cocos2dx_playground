#include "step_typetype_label_node_BasicScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace
{
	const char* PLIST_Path = "textures/step_typetype/step_typetype_textures.plist";
	const char* TEXTURE_Path = "textures/step_typetype/step_typetype_textures.png";
}

namespace step_typetype
{
	namespace label_node
	{
		LabelScene::LabelScene() : mKeyboardListener( nullptr ) {}

		Scene* LabelScene::create()
		{
			auto ret = new ( std::nothrow ) LabelScene();
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

		bool LabelScene::init()
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

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
				label->setPosition( Vec2(
					visibleOrigin.x + ( label->getContentSize().width * 0.5f )
					, visibleOrigin.y + visibleSize.height - ( label->getContentSize().height * 0.5f )
				) );
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
			// Label
			//
			{
				auto label = Label::createWithTTF( "Label Test... o_o", "fonts/NanumSquareR.ttf", 30, Size::ZERO, TextHAlignment::CENTER );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			return true;
		}

		void LabelScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( LabelScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void LabelScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void LabelScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_typetype::RootScene::create() );
				return;
			}
		}
	}
}
