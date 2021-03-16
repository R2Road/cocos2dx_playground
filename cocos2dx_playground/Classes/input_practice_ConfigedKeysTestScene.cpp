#include "input_practice_ConfigedKeysTestScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_StringTable.h"
#include "CPG_InputDelegator.h"
#include "CPG_Input_BasicCollector.h"
#include "CPG_InputKeyMap.h"
#include "cpg_input_KeyViewer.h"
#include "CPG_InputTest_KeyMapConfigHelper.h"
#include "CPG_Input_KeyCodeNames.h"

#include "input_practice_RootScene.h"
#include "input_practice_Setting.h"

USING_NS_CC;

namespace
{
	const int key_viewer_count = 10;
	const float key_viewer_margin = 4.f;
}

namespace input_practice
{
	ConfigedKeysTestScene::ConfigedKeysTestScene() :
		mInputCollector()
		, mKeyViewerList()
		, mKeyViewer_StartPosition()
	{}

	Scene* ConfigedKeysTestScene::create()
	{
		auto ret = new ( std::nothrow ) ConfigedKeysTestScene();
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

	bool ConfigedKeysTestScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleSize = _director->getVisibleSize();
		const auto visibleOrigin = _director->getVisibleOrigin();

		cpg::input::KeyMapConfigHelper key_map_config_helper;
		key_map_config_helper.Load( input_practice::Setting::getKeyMapFileName().c_str() );

		//
		// summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << "\n";
			ss << "\n";
			for( const auto& h : key_map_config_helper.GetContainer() )
			{
				ss << "[ " << h.Name << " : " << cpg::input::KeyCodeNames::get( h.CocosKeyCode ) << " ]" << std::endl;
			}

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
			auto label = Label::createWithTTF( "Exit", cpg::StringTable::GetFontPath(), 10 );
			label->setColor( Color3B::GREEN );

			auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_0.png", ui::Widget::TextureResType::PLIST );
			button->setColor( Color3B::GREEN );
			button->setScale9Enabled( true );
			button->setContentSize( label->getContentSize() + Size( 40.f, 4.f ) + Size( 40.f, 4.f ) );
			button->addTouchEventListener( CC_CALLBACK_2( ConfigedKeysTestScene::onExitButton, this ) );
			addChild( button, std::numeric_limits<int>::max() );
			button->setTitleLabel( label );

			button->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width - ( button->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height - ( button->getContentSize().height * 0.5f )
			) );
		}


		//
		// input
		//
		{
			auto input_delegator = cpg::input::Delegator::create( input_practice::Setting::getKeyAllowFileName().c_str() );
			addChild( input_delegator, 0 );
			{
				const auto key_map = cpg::input::KeyMap::create( input_practice::Setting::getKeyMapFileName().c_str() );

				mInputCollector = cpg::input::BasicCollector::create( key_map );
				input_delegator->addInputCollector( mInputCollector );
			}
		}


		//
		// key viewer
		//
		{
			cpg::input::KeyViewer* key_viewer = nullptr;
			mKeyViewer_StartPosition.set(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.1f )
			);
			for( int i = 0; i < key_viewer_count; ++i )
			{
				key_viewer = cpg::input::KeyViewer::create( { true, true }, key_map_config_helper );
				key_viewer->setPosition( mKeyViewer_StartPosition );
				key_viewer->setVisible( false );
				addChild( key_viewer, 1 );

				mKeyViewerList.push_back( key_viewer );
			}
			mKeyViewer_EndPosition.set(
				mKeyViewer_StartPosition.x
				, mKeyViewer_StartPosition.y + ( mKeyViewerList.front()->getContentSize().height * ( key_viewer_count - 1 ) )
			);

			// indicator
			auto indicator = Sprite::createWithSpriteFrameName( "white_2x2.png" );
			indicator->setScaleX( 200.f );
			indicator->setColor( Color3B::RED );
			indicator->setPosition( mKeyViewer_StartPosition );
			addChild( indicator, 0 );
		}

		//
		// Setup
		//
		scheduleUpdate();

		return true;
	}

	void ConfigedKeysTestScene::update( float dt )
	{
		if( mInputCollector->hasChanged() )
		{
			for( auto v : mKeyViewerList )
			{
				if( !v->isVisible() )
				{
					continue;
				}

				v->setPositionY( v->getPositionY() + v->getContentSize().height + key_viewer_margin );
				v->setVisible( mKeyViewer_EndPosition.y > v->getPositionY() );
			}

			for( auto v : mKeyViewerList )
			{
				if( v->isVisible() )
				{
					continue;
				}

				v->setVisible( true );
				v->setPosition( mKeyViewer_StartPosition );
				v->Setup( *mInputCollector );
				break;
			}
		}

		Scene::update( dt );
	}

	void ConfigedKeysTestScene::onExitButton( Ref* /*sender*/, ui::Widget::TouchEventType touch_event_type )
	{
		if( ui::Widget::TouchEventType::ENDED != touch_event_type )
		{
			return;
		}

		if( !isScheduled( schedule_selector( ConfigedKeysTestScene::update_forExit ) ) )
		{
			scheduleOnce( schedule_selector( ConfigedKeysTestScene::update_forExit ), 0.f );
		}
	}
	void ConfigedKeysTestScene::update_forExit( float /*dt*/ )
	{
		_director->replaceScene( input_practice::RootScene::create() );
	}
}
