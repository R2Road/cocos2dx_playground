#include "input_practice_AllowedKeysTestScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_StringTable.h"

#include "CPG_InputDelegator.h"
#include "CPG_InputTest_KeyboardInputObserver.h"
#include "CPG_Input_KeyCodeNames.h"

#include "input_practice_Setting.h"

USING_NS_CC;

namespace
{
	const int key_viewer_count = 10;
	const float key_viewer_margin = 4.f;
}

namespace input_practice
{
	AllowedKeysTestScene::AllowedKeysTestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mInputDelegator( nullptr )
		, mKeyViewer( nullptr )
		, mKeyStrings( 200, 0 )
		, mInputObserver( nullptr )
	{}

	Scene* AllowedKeysTestScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) AllowedKeysTestScene( back_to_the_previous_scene_callback );
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

	bool AllowedKeysTestScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		scheduleUpdate();

		const auto visibleSize = _director->getVisibleSize();
		const auto visibleOrigin = _director->getVisibleOrigin();

		//
		// summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << "    [ ... ] : do input allowed keys";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			label->setColor( Color3B::GREEN );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition( Vec2(
				visibleOrigin.x
				, visibleOrigin.y + visibleSize.height
			) );
			addChild( label, std::numeric_limits<int>::max() );
		}


		//
		// exit interface
		//
		{
			auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_0.png", ui::Widget::TextureResType::PLIST );
			button->setColor( Color3B::GREEN );
			button->getRendererNormal()->getTexture()->setAliasTexParameters();
			button->getRendererClicked()->getTexture()->setAliasTexParameters();
			button->getRendererDisabled()->getTexture()->setAliasTexParameters();
			button->setScale9Enabled( true );
			button->setContentSize( Size( 18.f, 12.f ) + Size( 40.f, 4.f ) + Size( 40.f, 4.f ) );
			button->addTouchEventListener( CC_CALLBACK_2( AllowedKeysTestScene::onExitButton, this ) );
			button->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width - ( button->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height - ( button->getContentSize().height * 0.5f )
			) );
			addChild( button, std::numeric_limits<int>::max() );

			// Title
			auto label = Label::createWithTTF( "Exit", cpg::StringTable::GetFontPath(), 10, Size::ZERO );
			label->setColor( Color3B::GREEN );
			button->setTitleLabel( label );
		}


		//
		// input
		//
		{
			mInputDelegator = cpg::input::Delegator::create( input_practice::Setting::getKeyAllowFileName().c_str() );
			addChild( mInputDelegator, 0 );

			mInputObserver = cpg::input_test::KeyboardInputObserver::create( input_practice::Setting::getKeyAllowFileName().c_str() );
			addChild( mInputObserver, 1 );
		}


		//
		// key viewer
		//
		{
			mKeyViewer = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::CENTER );
			mKeyViewer->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			addChild( mKeyViewer );
		}

		return true;
	}
	void AllowedKeysTestScene::update( float dt )
	{
		if( mInputObserver->inputFound() )
		{
			mKeyStrings.clear();
			for( std::size_t cur = 0; cur < step_rain_of_chaos::input::KeyCodeContainerSize; ++cur )
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
		if( ui::Widget::TouchEventType::ENDED == touch_event_type )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
