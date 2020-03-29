#include "step_mole_collision_ComponentScene.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <new>
#include <random>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"

#include "step_mole_CollisionComponent.h"
#include "step_mole_RootScene.h"

USING_NS_CC;

const int TAG_Actor = 20140416;

namespace step_mole
{
	namespace collision
	{
		ComponentScene::ComponentScene() :
			mKeyboardListener( nullptr )
			, mCollisionList()
		{}

		Scene* ComponentScene::create()
		{
			auto ret = new ( std::nothrow ) ComponentScene();
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

		bool ComponentScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
			const Size visibleMargin( 4.f, 4.f );

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
				ss << "[Mouse] : Push and Drag";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleMargin.width
					, visibleOrigin.y + visibleSize.height - visibleMargin.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}
			
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 15, 49, 101, 255 ) );
				addChild( background_layer, -1 );
			}

			//
			// Touch Pannel
			//
			{
				auto button = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->setContentSize( visibleSize );
				button->setOpacity( 150u );
				button->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				button->addTouchEventListener( CC_CALLBACK_2( ComponentScene::onButton, this ) );
				addChild( button );
			}

			//
			// Actor
			//
			{
				auto actor_root = Node::create();
				actor_root->setTag( TAG_Actor );
				actor_root->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				{
					// Pivot
					{
						auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
						pivot->setScale( 4.f );
						actor_root->addChild( pivot, 100 );
					}

					// View
					auto view_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
					view_node->setScale( 2.f );
					actor_root->addChild( view_node );
					{
						auto animation_object = Animation::create();
						animation_object->setDelayPerUnit( 0.2f );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

						auto animate_action = Animate::create( animation_object );

						auto repeat_action = RepeatForever::create( animate_action );

						view_node->runAction( repeat_action );
					}

					const Size margin( 3.f, 3.f );
					const float radius = ( view_node->getBoundingBox().size.height + margin.height ) * 0.5f;

					// Collision Component
					actor_root->addComponent( CollisionComponent::create( radius, true, true, true ) );
				}
				addChild( actor_root, 100 );
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

		void ComponentScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ComponentScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void ComponentScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void ComponentScene::addChild( Node* child, int localZOrder, int tag )
		{
			addCollision( child );
			Scene::addChild( child, localZOrder, tag );
		}
		void ComponentScene::addChild( Node* child, int localZOrder, const std::string &name )
		{
			addCollision( child );
			Scene::addChild( child, localZOrder, name );
		}
		void ComponentScene::removeChild( Node* child, bool cleanup /* = true */ )
		{
			removeCollision( child );
			Scene::removeChild( child, cleanup );
		}
		void ComponentScene::removeAllChildrenWithCleanup( bool cleanup )
		{
			mCollisionList.clear();
			Scene::removeAllChildrenWithCleanup( cleanup );
		}
		void ComponentScene::addCollision( cocos2d::Node* child )
		{
			auto target_component = child->getComponent( CollisionComponent::GetStaticName() );
			if( !target_component )
			{
				return;
			}

			mCollisionList.push_back( static_cast<CollisionComponent*>( target_component ) );
		}
		void ComponentScene::removeCollision( cocos2d::Node* child )
		{
			auto target_component = child->getComponent( CollisionComponent::GetStaticName() );
			if( !target_component )
			{
				return;
			}

			mCollisionList.remove( static_cast<CollisionComponent*>( target_component ) );
		}

		Node* ComponentScene::makeBullet()
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
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.1f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "bullet001_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "bullet001_02.png" ) );

					auto animate_action = Animate::create( animation_object );

					auto repeat_action = RepeatForever::create( animate_action );

					view_node->runAction( repeat_action );
				}

				const float radius = ( view_node->getBoundingBox().size.height ) * 0.5f;

				// Collision Component
				bullet_root_node->addComponent( CollisionComponent::create( radius, false, false, false ) );
			}

			return bullet_root_node;
		}

		void ComponentScene::collisionCheck()
		{
			auto actor_root = getChildByTag( TAG_Actor );
			auto actor_collision_component = static_cast<CollisionComponent*>( actor_root->getComponent( CollisionComponent::GetStaticName() ) );

			bool contact_success = false;
			for( const auto& c : mCollisionList )
			{
				if( c == actor_collision_component )
				{
					continue;
				}

				contact_success = actor_collision_component->Check( c );
				if( !contact_success )
				{
					continue;
				}

				break;
			}

			actor_collision_component->onContact( contact_success );
		}
		void ComponentScene::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			auto button = static_cast<ui::Button*>( sender );

			const auto actor_node = static_cast<Label*>( getChildByTag( TAG_Actor ) );

			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				actor_node->setPosition( button->getTouchBeganPosition() );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				actor_node->setPosition( button->getTouchMovePosition() );
			}
			else if( ui::Widget::TouchEventType::ENDED == touch_event_type )
			{
				actor_node->setPosition( button->getTouchEndPosition() );
			}

			collisionCheck();
		}

		void ComponentScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( step_mole::RootScene::create() );
				return;
			}
		}
	}
}
