#include "step02_collision_CollectionScene.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <new>
#include <sstream>

#include "ui/UIButton.h"

#include "cpg_CollisionComponent.h"
#include "step02_RootScene.h"

USING_NS_CC;

const int TAG_Actor = 20140416;
const int TAG_Bullet = 20200209;
const int TAG_MoveSpeed = 100;

namespace step02
{
	namespace collision
	{
		CollectionScene::CollectionScene() :
			mKeyboardListener( nullptr )
			, mCollisionList()
			, mKeyCodeCollector()
			, mMoveSpeed( 3 )
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

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}

			//
			// Move Speed View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 9 );
				label->setTag( TAG_MoveSpeed );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );

				updateMoveSpeedView();
			}
			
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 15, 49, 101, 255 ) );
				addChild( background_layer, 0 );
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
					actor_root->addComponent( cpg::CollisionComponent::create( radius, true, true, true ) );
				}
				addChild( actor_root, 100 );
			}

			//
			// Bullet
			//
			{
				auto bullet_root_node = makeBullet();
				bullet_root_node->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.7f )
				) );
				addChild( bullet_root_node, 101 );
			}

			return true;
		}

		void CollectionScene::onEnter()
		{
			Scene::onEnter();
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( CollectionScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased= CC_CALLBACK_2( CollectionScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void CollectionScene::update( float dt )
		{
			Vec2 input_vec2;
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_UP_ARROW ) )
			{
				input_vec2.y += 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_DOWN_ARROW ) )
			{
				input_vec2.y -= 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) )
			{
				input_vec2.x += 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_LEFT_ARROW ) )
			{
				input_vec2.x -= 1.f;
			}

			if( std::numeric_limits<float>::epsilon() < std::abs( input_vec2.x ) || std::numeric_limits<float>::epsilon() < std::abs( input_vec2.y ) )
			{
				//
				// Move
				//
				input_vec2.normalize();
				input_vec2.scale( mMoveSpeed );
				auto actor_root = getChildByTag( TAG_Actor );
				actor_root->setPosition( actor_root->getPosition() + input_vec2 );

				//
				// Collision Check
				//
				auto button_node = getChildByTag( TAG_Bullet );

				auto actor_collision_component = static_cast<cpg::CollisionComponent*>( actor_root->getComponent( cpg::CollisionComponent::GetStaticName() ) );
				auto bullet_collision_component = static_cast<cpg::CollisionComponent*>( button_node->getComponent( cpg::CollisionComponent::GetStaticName() ) );
				actor_collision_component->onContact( actor_collision_component->Check( bullet_collision_component ) );
			}

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
			auto target_component = child->getComponent( cpg::CollisionComponent::GetStaticName() );
			if( target_component )
			{
				mCollisionList.push_back( static_cast<cpg::CollisionComponent*>( target_component ) );
			}
			Scene::addChild( child, localZOrder, tag );
		}
		void CollectionScene::addChild( Node* child, int localZOrder, const std::string &name )
		{
			auto target_component = child->getComponent( cpg::CollisionComponent::GetStaticName() );
			if( target_component )
			{
				mCollisionList.push_back( static_cast<cpg::CollisionComponent*>( target_component ) );
			}
			Scene::addChild( child, localZOrder, name );
		}
		void CollectionScene::removeChild( Node* child, bool cleanup /* = true */ )
		{
			auto target_component = child->getComponent( cpg::CollisionComponent::GetStaticName() );
			if( target_component )
			{
				mCollisionList.remove( static_cast<cpg::CollisionComponent*>( target_component ) );
			}
			Scene::removeChild( child, cleanup );
		}
		void CollectionScene::removeAllChildrenWithCleanup( bool cleanup )
		{
			mCollisionList.clear();
			Scene::removeAllChildrenWithCleanup( cleanup );
		}

		Node* CollectionScene::makeBullet()
		{
			auto bullet_root_node = Node::create();
			bullet_root_node->setTag( TAG_Bullet );
			{
				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					bullet_root_node->addChild( pivot, 100 );
				}

				// View
				auto view_node = Sprite::createWithSpriteFrameName( "bullet001_01.png" );
				view_node->setTag( TAG_Bullet );
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
				bullet_root_node->addComponent( cpg::CollisionComponent::create( radius, true, false, false ) );
			}

			return bullet_root_node;
		}

		void CollectionScene::updateMoveSpeedView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_MoveSpeed ) );
			label->setString( StringUtils::format( "MoveSpeed : %d", mMoveSpeed ) );
		}

		void CollectionScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step02::RootScene::create() );
		}
		void CollectionScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( CollectionScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( CollectionScene::updateForExit ), 0.f );
				}
				return;
			}

			if( EventKeyboard::KeyCode::KEY_1 == keycode )
			{
				mMoveSpeed = std::min( 10, mMoveSpeed + 1 );
				updateMoveSpeedView();
				return;
			}
			if( EventKeyboard::KeyCode::KEY_2 == keycode )
			{
				mMoveSpeed = std::max( 1, mMoveSpeed - 1 );
				updateMoveSpeedView();
				return;
			}

			mKeyCodeCollector.onKeyPressed( keycode );
		}
		void CollectionScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode
				|| EventKeyboard::KeyCode::KEY_1 == keycode 
				|| EventKeyboard::KeyCode::KEY_2 == keycode )
			{
				return;
			}

			mKeyCodeCollector.onKeyReleased( keycode );
		}
	}
}
