#include "Research_Input_AllowedKeysTestScene.h"

#include <sstream>

#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

#include "Research_Input_RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_InputTest_KeyboardInputObserver.h"
#include "CPG_Input_KeyCodeNames.h"

#include "Research_Setting.h"

USING_NS_CC;

namespace research
{
	namespace input
	{
		const int key_viewer_count = 10;
		const float key_viewer_margin = 4.f;

		AllowedKeysTestScene::AllowedKeysTestScene() :
			mInputDelegator( nullptr )
			, mKeyViewer( nullptr )
			, mKeyStrings( 200, 0 )
			, mInputObserver( nullptr )
		{}

		Scene* AllowedKeysTestScene::create()
		{
			auto ret = new ( std::nothrow ) AllowedKeysTestScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			ret->autorelease();
			ret->scheduleUpdate();

			return ret;
		}

		bool AllowedKeysTestScene::init()
		{
			if( !Scene::init() )
				return false;

			auto visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();

			//
			// summury
			//
			{
				std::stringstream ss;
				ss << "+ Allowed Keys Test Scene";
				ss << "    [ ... ] : do input allowed keys";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					origin.x
					, origin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}


			//
			// exit interface
			//
			{
				auto label = Label::createWithTTF( "Exit", "fonts/arial.ttf", 10, Size::ZERO, TextHAlignment::CENTER );
				label->setColor( Color3B::GREEN );

				auto button = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_1.png", ui::Widget::TextureResType::PLIST );
				button->setColor( Color3B::GREEN );
				button->getRendererNormal()->getTexture()->setAliasTexParameters();
				button->getRendererClicked()->getTexture()->setAliasTexParameters();
				button->getRendererDisabled()->getTexture()->setAliasTexParameters();
				button->setScale9Enabled( true );
				button->setContentSize( label->getContentSize() + Size( 40.f, 4.f ) + Size( 40.f, 4.f ) );
				button->addTouchEventListener( CC_CALLBACK_2( AllowedKeysTestScene::onExitButton, this ) );
				addChild( button, 9999 );
				button->setTitleLabel( label );

				button->setPosition( Vec2(
					origin.x + visibleSize.width - ( button->getContentSize().width * 0.5f )
					, origin.y + visibleSize.height - ( button->getContentSize().height * 0.5f )
				) );
			}


			//
			// input
			//
			mInputDelegator = cpg::input::Delegator::create( research::Setting::getKeyAllowFileName().c_str() );
			addChild( mInputDelegator, 0 );

			mInputObserver = cpg::input_test::KeyboardInputObserver::create( research::Setting::getKeyAllowFileName().c_str() );
			addChild( mInputObserver, 1 );


			//
			// key viewer
			//
			mKeyViewer = Label::createWithTTF( "", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
			mKeyViewer->setPosition( Vec2(
				origin.x + ( visibleSize.width * 0.5f )
				, origin.y + ( visibleSize.height * 0.5f )
			) );
			addChild( mKeyViewer, 9999 );

			return true;
		}
		void AllowedKeysTestScene::update( float dt )
		{
			if( mInputObserver->inputFound() )
			{
				mKeyStrings.clear();
				for( std::size_t cur = 0; cur < cpg::input::AllowedKeys::ContainerSize; ++cur )
				{
					if( mInputDelegator->isActiveKey( static_cast<cocos2d::EventKeyboard::KeyCode>( cur ) ) )
					{
						mKeyStrings += cpg::input::KeyCodeNames::get( static_cast<cocos2d::EventKeyboard::KeyCode>( cur ) );
						mKeyStrings += "\n";
					}
				}
				mKeyViewer->setString( mKeyStrings );
			}

			Scene::update( dt );
		}


		void AllowedKeysTestScene::onExitButton( Ref* /*sender*/, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::ENDED != touch_event_type )
				return;

			if( !isScheduled( schedule_selector( AllowedKeysTestScene::update_forExit ) ) )
				scheduleOnce( schedule_selector( AllowedKeysTestScene::update_forExit ), 0.f );
		}
		void AllowedKeysTestScene::update_forExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}
	}
}
