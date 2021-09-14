#include "step_mole_collision_ComponentScene.h"

#include <algorithm>
#include <numeric>
#include <new>
#include <random>

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

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_mole_CircleCollisionComponent.h"
#include "step_mole_CircleCollisionComponentConfig.h"

USING_NS_CC;

namespace
{
	const int Z_Bullet = 100;
	const int Z_Actor = 101;
}

namespace step_mole
{
	namespace collision
	{
		ComponentScene::ComponentScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mActorNode( nullptr )
			, mCollisionComponentList()
		{}

		Scene* ComponentScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) ComponentScene( back_to_the_previous_scene_callback );
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

		bool ComponentScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();
			const Vec2 visibleCenter(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
			);

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Mouse] : Push and Drag";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( 0.f, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}
			
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 15, 49, 101, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Touch Pannel
			//
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->setContentSize( visibleSize );
				button->setOpacity( 150u );
				button->setPosition( visibleCenter );
				button->addTouchEventListener( CC_CALLBACK_2( ComponentScene::onButton, this ) );
				addChild( button );
			}

			//
			// Actor
			//
			{
				mActorNode = Node::create();
				mActorNode->setPosition( visibleCenter );
				{
					// Pivot
					{
						auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
						pivot->setScale( 4.f );
						mActorNode->addChild( pivot, std::numeric_limits<int>::max() );
					}

					// View
					auto view_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
					view_node->setScale( _director->getContentScaleFactor() );
					mActorNode->addChild( view_node );
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
					mActorNode->addComponent( CircleCollisionComponent::create( radius, Vec2::ZERO, CircleCollisionComponentConfig{ true, true, true } ) );
				}
				addChild( mActorNode, Z_Actor );
			}

			//
			// Bullet
			//
			{
				const float negative_range = visibleSize.height * 0.3f;
				const float position_margin = 10.f;

				std::random_device rd;
				std::mt19937 randomEngine( rd() );
				std::uniform_int_distribution<> distX( static_cast<int>( visibleOrigin.x + position_margin ), static_cast<int>( visibleSize.width - position_margin ) );
				std::uniform_int_distribution<> distY( static_cast<int>( visibleOrigin.y + position_margin ), static_cast<int>( visibleSize.height - position_margin ) );

				int current_bullet_count = 0;
				Vec2 new_bullet_position;
				bool collision_enable = true;
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

					auto bullet_root_node = makeBullet( collision_enable );
					bullet_root_node->setPosition( visibleOrigin + new_bullet_position );
					addChild( bullet_root_node, Z_Bullet );

					mCollisionComponentList.push_back( static_cast<CircleCollisionComponent*>(
						bullet_root_node->getComponent( CircleCollisionComponent::GetStaticName() )
					) );

					collision_enable = !collision_enable;
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
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ComponentScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		Node* ComponentScene::makeBullet( const bool collision_enable )
		{
			auto bullet_root_node = Node::create();
			{
				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					bullet_root_node->addChild( pivot, std::numeric_limits<int>::max() );
				}

				// View
				auto view_node = Sprite::createWithSpriteFrameName( "step_mole_target_wait_0.png" );
				view_node->setScale( _director->getContentScaleFactor() );
				view_node->setColor( collision_enable ? Color3B::WHITE : Color3B( 90, 90, 90 ) );
				bullet_root_node->addChild( view_node );
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.1f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_wait_0.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_target_wait_1.png" ) );

					auto animate_action = Animate::create( animation_object );

					auto repeat_action = RepeatForever::create( animate_action );

					view_node->runAction( repeat_action );
				}

				const float radius = ( view_node->getBoundingBox().size.height ) * 0.5f;

				// Collision Component
				auto circle_collision_component = CircleCollisionComponent::create( radius, Vec2( 0.f, -3.f ), CircleCollisionComponentConfig{ true, true, true} );
				circle_collision_component->setEnabled( collision_enable );
				bullet_root_node->addComponent( circle_collision_component );
			}

			return bullet_root_node;
		}

		void ComponentScene::collisionCheck()
		{
			auto actor_collision_component = static_cast<CircleCollisionComponent*>( mActorNode->getComponent( CircleCollisionComponent::GetStaticName() ) );

			bool contact_success = false;
			for( const auto& c : mCollisionComponentList )
			{
				if( c == actor_collision_component )
				{
					continue;
				}

				auto temp_contact_success = actor_collision_component->Check( c );
				c->onContact( temp_contact_success );
				
				contact_success = contact_success | temp_contact_success;
			}

			actor_collision_component->onContact( contact_success );
		}
		void ComponentScene::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			auto button = static_cast<ui::Button*>( sender );

			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				mActorNode->setPosition( button->getTouchBeganPosition() );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				mActorNode->setPosition( button->getTouchMovePosition() );
			}
			else if( ui::Widget::TouchEventType::ENDED == touch_event_type )
			{
				mActorNode->setPosition( button->getTouchEndPosition() );
			}

			collisionCheck();
		}

		void ComponentScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
