#include "Step01_Game_PathFinder_PlayScene.h"

#include <new>
#include <sstream>
#include <random>

#include "Step01_Game_PathFinder_TitleScene.h"

USING_NS_CC;

namespace step01
{
	namespace game
	{
		namespace pathfinder
		{
			PlayScene::PlayScene() : mKeyboardListener( nullptr ), mTerrainData() {}

			Scene* PlayScene::create()
			{
				auto ret = new ( std::nothrow ) PlayScene();
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

			bool PlayScene::init()
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
					ss << "[ESC] : Return to Title";

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
				// Terrain
				//
				{
					mTerrainData.load( 5, 5 );

					const Size tile_size( 64.f / Director::getInstance()->getContentScaleFactor(), 64.f / Director::getInstance()->getContentScaleFactor() );
					const Vec2 pivot_position( tile_size.width * 0.5f, tile_size.height * 0.5f );
					auto terrain_layer = Layer::create();
					terrain_layer->setContentSize( Size( tile_size.width * mTerrainData.getWidth(), tile_size.height * mTerrainData.getHeight() ) );
					terrain_layer->setPosition( Vec2(
						visibleOrigin.x + ( ( visibleSize.width - terrain_layer->getContentSize().width ) * 0.5f )
						, visibleOrigin.y + ( ( visibleSize.height - terrain_layer->getContentSize().height ) * 0.5f )
					) );
					addChild( terrain_layer );

					Sprite* temp = nullptr;
					for( int ty = 0; ty < mTerrainData.getWidth(); ++ty )
					{
						for( int tx = 0; tx < mTerrainData.getWidth(); ++tx )
						{
							temp = mTerrainData.get( tx, ty )
								? Sprite::createWithSpriteFrameName( "guide_01_1.png" )
								: Sprite::createWithSpriteFrameName( "guide_01_2.png" );
							temp->setPosition( pivot_position + Vec2( ( tx * tile_size.width ), ( ty * tile_size.height ) ) );

							terrain_layer->addChild( temp );
						}
					}
				}

				return true;
			}

			void PlayScene::onEnter()
			{
				Scene::onEnter();

				mKeyboardListener = EventListenerKeyboard::create();
				mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
				getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
			}
			void PlayScene::onExit()
			{
				assert( mKeyboardListener );
				getEventDispatcher()->removeEventListener( mKeyboardListener );
				mKeyboardListener = nullptr;

				Node::onExit();
			}

			void PlayScene::updateForExit( float /*dt*/ )
			{
				Director::getInstance()->replaceScene( game::pathfinder::TitleScene::create() );
			}

			void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
			{
				switch( keycode )
				{
				case EventKeyboard::KeyCode::KEY_ESCAPE:
					if( !isScheduled( schedule_selector( PlayScene::updateForExit ) ) )
						scheduleOnce( schedule_selector( PlayScene::updateForExit ), 0.f );
					break;

				default:
					CCLOG( "Key Code : %d", keycode );
				}
			}
		} // namespace pathfinder
	}
}
