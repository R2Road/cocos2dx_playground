#include "step_rain_of_chaos_collision_CollectionScene.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <new>
#include <random>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "step_mole_CircleCollisionComponent.h"
#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_rain_of_chaos_RootScene.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace collision
	{
		CollectionScene::CollectionScene() :
			mKeyboardListener( nullptr )
			, mCollisionCollection()
		{}

		Scene* CollectionScene::create()
		{
			auto ret = new ( std::nothrow ) CollectionScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}
			else
			{
				ret->scheduleUpdate();
				ret->autorelease();
			}

			return ret;
		}

		bool CollectionScene::init()
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
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow Key] : Move Actor";
				ss << std::endl;
				ss << std::endl;
				ss << "[1] : Speed Up";
				ss << std::endl;
				ss << "[2] : Speed Down";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 130, 49, 29, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// Bullet
			//
			{
				const Vec2 visibleCenter(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				);
				const float negative_range = ( visibleSize.height - visibleOrigin.y ) * 0.3f;
				const float position_margin = 10.f;

				std::random_device rd;
				std::mt19937 randomEngine( rd() );
				std::uniform_int_distribution<> distX( static_cast<int>( visibleOrigin.x + position_margin ), static_cast<int>( visibleSize.width - position_margin ) );
				std::uniform_int_distribution<> distY( static_cast<int>( visibleOrigin.y + position_margin ), static_cast<int>( visibleSize.height - position_margin ) );

				int current_bullet_count = 0;
				Vec2 new_bullet_position;
				while( current_bullet_count < 50 )
				{
					new_bullet_position.set(
						distX( randomEngine )
						,distY( randomEngine )
					);

					if( negative_range >= visibleCenter.distance( new_bullet_position ) )
					{
						continue;
					}

					auto bullet_root_node = makeBullet();
					bullet_root_node->setPosition( visibleOrigin + new_bullet_position );
					addChild( bullet_root_node, 101 );

					++current_bullet_count;
				}
			}

			return true;
		}

		void CollectionScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( CollectionScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void CollectionScene::update( float dt )
		{
			Scene::update( dt );
		}
		void CollectionScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void CollectionScene::addChild( Node* child, int localZOrder, int tag )
		{
			addCollision( child );
			Scene::addChild( child, localZOrder, tag );
		}
		void CollectionScene::addChild( Node* child, int localZOrder, const std::string &name )
		{
			addCollision( child );
			Scene::addChild( child, localZOrder, name );
		}
		void CollectionScene::removeChild( Node* child, bool cleanup /* = true */ )
		{
			removeCollision( child );
			Scene::removeChild( child, cleanup );
		}
		void CollectionScene::removeAllChildrenWithCleanup( bool cleanup )
		{
			mCollisionCollection.Clear();
			Scene::removeAllChildrenWithCleanup( cleanup );
		}

		void CollectionScene::addCollision( cocos2d::Node* child )
		{
			auto target_component = child->getComponent( step_mole::CircleCollisionComponent::GetStaticName() );
			if( !target_component )
			{
				return;
			}

			mCollisionCollection.Add( static_cast<step_mole::CircleCollisionComponent*>( target_component ) );
		}
		void CollectionScene::removeCollision( cocos2d::Node* child )
		{
			auto target_component = child->getComponent( step_mole::CircleCollisionComponent::GetStaticName() );
			if( !target_component )
			{
				return;
			}

			mCollisionCollection.Remove( static_cast<step_mole::CircleCollisionComponent*>( target_component ) );
		}

		Node* CollectionScene::makeBullet()
		{
			auto bullet_root_node = Node::create();
			{
				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					bullet_root_node->addChild( pivot, 100 );
				}

				// View
				auto view_node = Sprite::createWithSpriteFrameName( "bullet001_01.png" );
				bullet_root_node->addChild( view_node );

				const float radius = ( view_node->getBoundingBox().size.height ) * 0.5f;

				// Collision Component
				bullet_root_node->addComponent( step_mole::CircleCollisionComponent::create( radius, Vec2::ZERO, step_mole::CircleCollisionComponentConfig{ false, false, false } ) );
			}

			return bullet_root_node;
		}

		void CollectionScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( step_rain_of_chaos::RootScene::create() );
				return;
			}
		}
	}
}
