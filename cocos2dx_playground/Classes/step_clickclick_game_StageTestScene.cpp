#include "step_clickclick_game_StageTestScene.h"

#include <new>
#include <sstream>
#include <numeric>

#include "step_clickclick_RootScene.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		StageTestScene::StageTestScene() : mKeyboardListener( nullptr ) {}

		Scene* StageTestScene::create()
		{
			auto ret = new ( std::nothrow ) StageTestScene();
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

		bool StageTestScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

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
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 0, 41, 13, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// Stage
			//
			{
				const int stage_width = 3;
				const int stage_height = 3;
				const Size tile_size( 32.f, 32.f );
				const Size margin_size( 2.f, 2.f );
				const Size stage_size(
					( stage_width * tile_size.width ) + ( ( stage_width - 1 ) * margin_size.width )
					,( stage_height * tile_size.height ) + ( ( stage_height - 1 ) * margin_size.height )
				);
				const Vec2 pivot_position( stage_size.width * -0.5f, stage_size.height * -0.5f );

				auto root_node = Node::create();
				root_node->setContentSize( stage_size );
				root_node->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( root_node );
				{
					// Pivot
					{
						auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
						pivot->setScale( 2.f );
						root_node->addChild( pivot, std::numeric_limits<int>::max() );
					}

					// Background Guide
					{
						auto pivot = LayerColor::create( Color4B( 0u, 0u, 0u, 100u ), root_node->getContentSize().width, root_node->getContentSize().height );
						pivot->setPosition( pivot_position );
						root_node->addChild( pivot, std::numeric_limits<int>::min() );
					}
				}
			}

			return true;
		}

		void StageTestScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( StageTestScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void StageTestScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void StageTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_clickclick::RootScene::create() );
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	} // namespace game
} // namespace step_clickclick
