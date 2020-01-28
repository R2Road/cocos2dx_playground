#include "Step01_Json_LoadNSaveScene.h"

#include <new>
#include <sstream>

#include "Step01_RootScene.h"

USING_NS_CC;

namespace step01
{
	namespace json
	{
		LoadNSaveScene::LoadNSaveScene() : mKeyboardListener( nullptr ) {}
		LoadNSaveScene::~LoadNSaveScene()
		{
			mKeyboardListener->release();
		}

		Scene* LoadNSaveScene::create()
		{
			auto ret = new ( std::nothrow ) LoadNSaveScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool LoadNSaveScene::init()
		{
			if( !Scene::init() )
				return false;

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();


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
				ss << "<Json File Path> : " << cocos2d::FileUtils::getInstance()->getWritablePath();

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}

			//
			// Keyboard Listener
			//
			{
				mKeyboardListener = EventListenerKeyboard::create();
				mKeyboardListener->onKeyPressed = CC_CALLBACK_2( LoadNSaveScene::onKeyPressed, this );
				mKeyboardListener->retain();
			}

			return true;
		}

		void LoadNSaveScene::onEnter()
		{
			Scene::onEnter();
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void LoadNSaveScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			Node::onExit();
		}

		void LoadNSaveScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}
		void LoadNSaveScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( LoadNSaveScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( LoadNSaveScene::updateForExit ), 0.f );
				}
			}
		}
	}
}
