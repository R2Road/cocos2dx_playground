#include "step02_collision_BasicScene.h"

#include <new>
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
#include "base/ccUTF8.h"

#include "step_rain_of_chaos_RootScene.h"

USING_NS_CC;

const int TAG_Actor = 20140416;
const int TAG_Bullet = 20200209;
const int TAG_Distance = 888;
const int TAG_CollisionIndicator = 999;
const int TAG_MoveSpeed = 100;

class RadiusData : public cocos2d::Ref
{
public:
	RadiusData( const float radius ) : mRadius( radius ) {}

	float GetRadius() const { return mRadius; }

private:
	float mRadius;
};

namespace step02
{
	namespace collision
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr ), mMoveSpeed( 3 )
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
				ret->scheduleUpdate();
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
				auto actor_root_node = Node::create();
				actor_root_node->setTag( TAG_Actor );
				actor_root_node->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				{
					// Pivot
					{
						auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
						pivot->setScale( 4.f );
						actor_root_node->addChild( pivot, 100 );
					}

					// View
					auto view_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
					view_node->setScale( 2.f );
					actor_root_node->addChild( view_node );
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

					// Radius View
					{
						auto label = Label::createWithTTF( StringUtils::format( "%.2f", radius ), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
						label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
						label->setPositionX( radius + margin.width );
						actor_root_node->addChild( label );
					}

					// Collision Guide
					{
						auto guide = Sprite::createWithSpriteFrameName( "guide_02_4.png" );
						guide->setScale( radius / ( guide->getContentSize().width * 0.5f ) );
						actor_root_node->addChild( guide );
					}

					// Collision Indicator
					{
						auto collision_indicator_node = Sprite::createWithSpriteFrameName( "guide_02_7.png" );
						collision_indicator_node->setTag( TAG_CollisionIndicator );
						collision_indicator_node->setScale( radius / ( collision_indicator_node->getContentSize().width * 0.5f ) );
						collision_indicator_node->setVisible( false );
						actor_root_node->addChild( collision_indicator_node );
					}

					// Radius Data
					{
						auto radius_data( new RadiusData( radius ) );
						actor_root_node->setUserObject( radius_data );
						radius_data->release();
					}
				}
				addChild( actor_root_node, 100 );
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

					const Size margin( 3.f, 3.f );
					const float radius = ( view_node->getBoundingBox().size.height ) * 0.5f;

					// Radius View
					{
						auto label = Label::createWithTTF( StringUtils::format( "%.2f", radius ), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
						label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
						label->setPositionX( radius + margin.width );
						bullet_root_node->addChild( label );
					}

					// Radius Data
					{
						auto radius_data( new RadiusData( radius ) );
						bullet_root_node->setUserObject( radius_data );
						radius_data->release();
					}
				}
				addChild( bullet_root_node, 101 );
			}

			//
			// Distance
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 12 );
				label->setTag( TAG_Distance );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y
				) );
				addChild( label, 200 );

				updateDistance();
			}

			//
			// Move Speed View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 12 );
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

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( BasicScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void BasicScene::update( float dt )
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
				auto actor_root_node = getChildByTag( TAG_Actor );
				actor_root_node->setPosition( actor_root_node->getPosition() + input_vec2 );

				//
				// Distance View
				//
				updateDistance();

				//
				// Collision Check
				//
				auto bullet_root_node = getChildByTag( TAG_Bullet );

				const float distance = actor_root_node->getPosition().distance( bullet_root_node->getPosition() );

				const auto actor_radius_data = static_cast<RadiusData*>( actor_root_node->getUserObject() );
				const auto bullet_radius_data = static_cast<RadiusData*>( bullet_root_node->getUserObject() );
				const float contact_limit_distance = actor_radius_data->GetRadius() + bullet_radius_data->GetRadius();

				actor_root_node->getChildByTag( TAG_CollisionIndicator )->setVisible( distance <= contact_limit_distance );
			}

			Scene::update( dt );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			Scene::onExit();
		}

		void BasicScene::updateDistance()
		{
			const auto bullet_node = static_cast<Label*>( getChildByTag( TAG_Bullet ) );
			const auto actor_node = static_cast<Label*>( getChildByTag( TAG_Actor ) );

			const auto distance = bullet_node->getPosition().distance( actor_node->getPosition() );

			auto label = static_cast<Label*>( getChildByTag( TAG_Distance ) );
			label->setString( StringUtils::format( "Distance : %.2f", distance ) );
		}

		void BasicScene::updateMoveSpeedView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_MoveSpeed ) );
			label->setString( StringUtils::format( "MoveSpeed : %d", mMoveSpeed ) );
		}

		void BasicScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_rain_of_chaos::RootScene::create() );
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
		void BasicScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
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
