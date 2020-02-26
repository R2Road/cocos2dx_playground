#include "step02_collision_BasicScene.h"

#include <new>
#include <sstream>

#include "ui/UIButton.h"

#include "cpg_CollisionComponent.h"
#include "Step02_RootScene.h"

USING_NS_CC;

const int TAG_Actor = 20140416;
const int TAG_Bullet = 20200209;
const int TAG_Distance = 888;

namespace step02
{
	namespace collision
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr ), mButtonMoveOffset( Vec2::ZERO ), mCollisionList()
		{}

		Scene* BasicScene::create()
		{
			auto ret = new ( std::nothrow ) BasicScene();
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

		bool BasicScene::init()
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
				ss << "[Mouse] : Do Click and Drag";
				ss << std::endl;
				ss << "[1] : Position Reset";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					actor_root->addChild( pivot, 100 );

					// View
					auto player_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
					player_node->setScale( 2.f );
					actor_root->addChild( player_node );
					{
						auto animation_object = Animation::create();
						animation_object->setDelayPerUnit( 0.2f );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

						auto animate_action = Animate::create( animation_object );

						auto repeat_action = RepeatForever::create( animate_action );

						player_node->runAction( repeat_action );
					}

					const Size margin( 3.f, 3.f );
					const float radius = ( player_node->getBoundingBox().size.height + margin.height ) * 0.5f;

					// Button
					auto button = ui::Button::create( "guide_02_4.png", "guide_02_5.png", "guide_02_6.png", ui::Widget::TextureResType::PLIST );
					button->addTouchEventListener( CC_CALLBACK_2( BasicScene::onButton, this ) );
					button->setScale( radius / ( button->getContentSize().width * 0.5f ) );
					actor_root->addChild( button );

					// Collision Component
					actor_root->addComponent( cpg::CollisionComponent::create( radius ) );
				}
				addChild( actor_root, 100 );
			}

			//
			// Bullet
			//
			{
				auto bullet_root_node = Node::create();
				bullet_root_node->setTag( TAG_Bullet );
				bullet_root_node->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.7f )
				) );
				{
					// Pivot
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 4.f );
					bullet_root_node->addChild( pivot, 100 );

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

					const Size margin( 0.f, 0.f );
					const float radius = ( view_node->getBoundingBox().size.height + margin.height ) * 0.5f;

					// Collision Component
					bullet_root_node->addComponent( cpg::CollisionComponent::create( radius ) );
				}
				addChild( bullet_root_node, 101 );
			}

			//
			// Distance
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
				label->setTag( TAG_Distance );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 200 );

				updateDistance();
			}

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			Node::onExit();
		}

		void BasicScene::addChild( Node* child, int localZOrder, int tag )
		{
			auto target_component = child->getComponent( cpg::CollisionComponent::GetStaticName() );
			if( target_component )
			{
				mCollisionList.push_back( static_cast<cpg::CollisionComponent*>( target_component ) );
			}
			Scene::addChild( child, localZOrder, tag );
		}
		void BasicScene::addChild( Node* child, int localZOrder, const std::string &name )
		{
			auto target_component = child->getComponent( cpg::CollisionComponent::GetStaticName() );
			if( target_component )
			{
				mCollisionList.push_back( static_cast<cpg::CollisionComponent*>( target_component ) );
			}
			Scene::addChild( child, localZOrder, name );
		}
		void BasicScene::removeChild( Node* child, bool cleanup /* = true */ )
		{
			auto target_component = child->getComponent( cpg::CollisionComponent::GetStaticName() );
			if( target_component )
			{
				mCollisionList.remove( static_cast<cpg::CollisionComponent*>( target_component ) );
			}
			Scene::removeChild( child, cleanup );
		}
		void BasicScene::removeAllChildrenWithCleanup( bool cleanup )
		{
			mCollisionList.clear();
			Scene::removeAllChildrenWithCleanup( cleanup );
		}

		void BasicScene::updateDistance()
		{
			const auto bullet_node = static_cast<Label*>( getChildByTag( TAG_Bullet ) );
			const auto actor_node = static_cast<Label*>( getChildByTag( TAG_Actor ) );

			const auto distance = bullet_node->getPosition().distance( actor_node->getPosition() );

			auto label = static_cast<Label*>( getChildByTag( TAG_Distance ) );
			label->setString( StringUtils::format( "Distance : %.2f", distance ) );
		}

		void BasicScene::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );
				auto actor_root = getChildByTag( TAG_Actor );

				mButtonMoveOffset = actor_root->getPosition() - button->getTouchBeganPosition();
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );
				auto actor_root = getChildByTag( TAG_Actor );

				actor_root->setPosition( button->getTouchMovePosition() + mButtonMoveOffset );

				updateDistance();
			}
		}

		void BasicScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step02::RootScene::create() );
		}
		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( BasicScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( BasicScene::updateForExit ), 0.f );
				}
				return;
			}

			if( EventKeyboard::KeyCode::KEY_1 == keycode )
			{
				const auto visibleSize = Director::getInstance()->getVisibleSize();
				const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

				auto node = getChildByTag( TAG_Actor );
				node->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
			}
		}
	}
}
