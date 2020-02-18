#include "Research_Input_ConfigedKeysTestScene.h"

#include <sstream>

#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

#include "Step99_RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_Input_BasicCollector.h"
#include "CPG_InputKeyMap.h"
#include "CPG_InputTest_KeyViewer.h"
#include "CPG_InputTest_KeyMapConfigHelper.h"
#include "CPG_Input_KeyCodeNames.h"

#include "Research_Setting.h"

USING_NS_CC;

namespace research
{
	namespace input
	{
		const int key_viewer_count = 10;
		const float key_viewer_margin = 4.f;

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
				return nullptr;
			}
			else
			{
				ret->autorelease();
			}

			ret->scheduleUpdate();
			return ret;
		}

		bool ConfigedKeysTestScene::init()
		{
			if( !Scene::init() )
				return false;

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

			cpg::input_test::KeyMapConfigHelper key_map_config_helper;
			key_map_config_helper.load( research::Setting::getKeyMapFileName().c_str() );

			//
			// summury
			//
			{
				std::stringstream ss;
				ss << "+ Configed Keys Test Scene";
				ss << "\n";
				ss << "\n";
				for( const auto& h : key_map_config_helper.getContainer() )
					ss << "[ " << h.mName << " : " << cpg::input::KeyCodeNames::get( h.mKeycode ) << " ]" << std::endl;

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
				button->addTouchEventListener( CC_CALLBACK_2( ConfigedKeysTestScene::onExitButton, this ) );
				addChild( button, 9999 );
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
				auto input_delegator = cpg::input::Delegator::create( research::Setting::getKeyAllowFileName().c_str() );
				addChild( input_delegator, 0 );

				const auto key_map = cpg::input::KeyMap::create( research::Setting::getKeyMapFileName().c_str() );

				mInputCollector = cpg::input::BasicCollector::create( key_map );
				input_delegator->addInputCollector( mInputCollector );
			}


			//
			// key viewer
			//
			{
				cpg::input_test::KeyViewer* key_viewer = nullptr;
				mKeyViewer_StartPosition.set(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.1f )
				);
				for( int i = 0; i < key_viewer_count; ++i )
				{
					key_viewer = cpg::input_test::KeyViewer::create( key_map_config_helper );
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
				auto indicator = Sprite::createWithSpriteFrameName( "empty_2x2.png" );
				indicator->setScaleX( 200.f );
				indicator->setColor( Color3B::RED );
				indicator->setPosition( mKeyViewer_StartPosition );
				addChild( indicator, 0 );
			}

			return true;
		}

		void ConfigedKeysTestScene::update( float dt )
		{
			if( mInputCollector->hasChanged() )
			{
				for( auto v : mKeyViewerList )
				{
					if( !v->isVisible() )
						continue;

					v->setPositionY( v->getPositionY() + v->getContentSize().height + key_viewer_margin );
					v->setVisible( mKeyViewer_EndPosition.y > v->getPositionY() );
				}

				for( auto v : mKeyViewerList )
				{
					if( v->isVisible() )
						continue;

					v->setVisible( true );
					v->setPosition( mKeyViewer_StartPosition );
					v->setup( mInputCollector );
					break;
				}
			}

			Scene::update( dt );
		}

		void ConfigedKeysTestScene::onExitButton( Ref* /*sender*/, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::ENDED != touch_event_type )
				return;

			if( !isScheduled( schedule_selector( ConfigedKeysTestScene::update_forExit ) ) )
				scheduleOnce( schedule_selector( ConfigedKeysTestScene::update_forExit ), 0.f );
		}
		void ConfigedKeysTestScene::update_forExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step99::RootScene::create() );
		}
	}
}
