#include "Step01_Tool_MapToolScene.h"

#include <new>
#include <sstream>

#include "ui/UIButton.h"

#include "Step01_RootScene.h"

USING_NS_CC;

const int TAG_Indicator = 20140416;

namespace step01
{
	namespace tool
	{
		MapToolScene::MapToolScene() :
			mKeyboardListener( nullptr )
			, mTerrainData()
		{}

		Scene* MapToolScene::create()
		{
			auto ret = new ( std::nothrow ) MapToolScene();
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

		bool MapToolScene::init()
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
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Return to Step 01 Root";

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
			// Terrain Data
			//
			{
				const int map_size_x = 5;
				const int map_size_y = 5;
				mTerrainData.reSize( map_size_x, map_size_y );
			}

			//
			// Terrain View
			//
			{
				const auto tile_size = SpriteFrameCache::getInstance()->getSpriteFrameByName( "guide_01_1.png" )->getRect().size;
				const Vec2 pivot_position( tile_size.width * 0.5f, tile_size.height * 0.5f );
				auto terrain_layer = Layer::create();
				terrain_layer->setContentSize( Size( tile_size.width * mTerrainData.getWidth(), tile_size.height * mTerrainData.getHeight() ) );
				terrain_layer->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - terrain_layer->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - terrain_layer->getContentSize().height ) * 0.5f )
				) );
				addChild( terrain_layer );

				ui::Button* temp = nullptr;
				for( int ty = 0; ty < mTerrainData.getHeight(); ++ty )
				{
					for( int tx = 0; tx < mTerrainData.getWidth(); ++tx )
					{
						temp = ui::Button::create( "guide_01_4.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
						{
							auto indicator = Sprite::createWithSpriteFrameName( "step01_game_tile_00.png" );
							indicator->setTag( TAG_Indicator );
							indicator->setVisible( false );
							indicator->setPosition( Vec2( temp->getContentSize().width * 0.5f, temp->getContentSize().height * 0.5f ) );
							temp->addChild( indicator );
						}
						temp->addTouchEventListener( CC_CALLBACK_2( MapToolScene::onButton, this ) );
						temp->setPosition( pivot_position + Vec2( ( tx * tile_size.width ), ( ty * tile_size.height ) ) );

						terrain_layer->addChild( temp );
					}
				}
			}

			return true;
		}

		void MapToolScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( MapToolScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void MapToolScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}


		void MapToolScene::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			auto button = static_cast<Node*>( sender );
			auto indicator = button->getChildByTag( 20140416 );
			indicator->setVisible( !indicator->isVisible() );
		}


		void MapToolScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step01::RootScene::create() );
		}
		void MapToolScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE != keycode )
			{
				return;
			}

			if( isScheduled( schedule_selector( MapToolScene::updateForExit ) ) )
			{
				return;
			}

			scheduleOnce( schedule_selector( MapToolScene::updateForExit ), 0.f );
		}
	} // namespace game
}
