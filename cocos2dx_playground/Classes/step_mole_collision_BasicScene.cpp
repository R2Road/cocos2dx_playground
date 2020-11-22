#include "step_mole_collision_BasicScene.h"

#include <new>
#include <numeric>
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
#include "ui/UIButton.h"

#include "cpg_StringTable.h"

#include "step_mole_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_Actor = 20140416;
	const int TAG_Bullet = 20200209;
	const int TAG_Distance = 888;
	const int TAG_CollisionIndicator = 999;

	const int Z_Bullet = 100;
	const int Z_Actor = 101;

	class RadiusData : public cocos2d::Ref
	{
	public:
		RadiusData( const float radius ) : mRadius( radius ) {}

		float GetRadius() const { return mRadius; }

	private:
		const float mRadius;
	};
}

namespace step_mole
{
	namespace collision
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr ) {}

		Scene* BasicScene::create()
		{
			auto ret = new ( std::nothrow ) BasicScene();
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

		bool BasicScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();
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

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Distance
			//
			{
				auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12 );
				label->setTag( TAG_Distance );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + visibleMargin.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Touch Pannel
			//
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->setContentSize( visibleSize );
				button->setOpacity( 150u );
				button->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				button->addTouchEventListener( CC_CALLBACK_2( BasicScene::onButton, this ) );
				addChild( button );
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
						actor_root_node->addChild( pivot, std::numeric_limits<int>::max() );
					}

					// View
					auto view_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
					view_node->setScale( _director->getContentScaleFactor() );
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
						auto label = Label::createWithTTF( StringUtils::format( "%.2f", radius ), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
				addChild( actor_root_node, Z_Actor );
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
						bullet_root_node->addChild( pivot, std::numeric_limits<int>::max() );
					}

					// View
					auto view_node = Sprite::createWithSpriteFrameName( "step_mole_target_wait_0.png" );
					view_node->setTag( TAG_Bullet );
					view_node->setScale( _director->getContentScaleFactor() );
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

					const Size margin( 3.f, 3.f );
					const float radius = ( view_node->getBoundingBox().size.height ) * 0.5f;

					// Radius View
					{
						auto label = Label::createWithTTF( StringUtils::format( "%.2f", radius ), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
						label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
						label->setPositionX( radius + margin.width );
						bullet_root_node->addChild( label );
					}

					// Collision Guide
					{
						auto guide = Sprite::createWithSpriteFrameName( "guide_02_4.png" );
						guide->setScale( radius / ( guide->getContentSize().width * 0.5f ) );
						bullet_root_node->addChild( guide );
					}

					// Radius Data
					{
						auto radius_data( new RadiusData( radius ) );
						bullet_root_node->setUserObject( radius_data );
						radius_data->release();
					}
				}
				addChild( bullet_root_node, Z_Bullet );
			}

			updateDistance();

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			Scene::onExit();
		}

		void BasicScene::collisionCheck()
		{
			auto actor_node = static_cast<Label*>( getChildByTag( TAG_Actor ) );
			const auto bullet_root_node = getChildByTag( TAG_Bullet );

			const float distance = actor_node->getPosition().distance( bullet_root_node->getPosition() );

			const auto actor_radius_data = static_cast<RadiusData*>( actor_node->getUserObject() );
			const auto bullet_radius_data = static_cast<RadiusData*>( bullet_root_node->getUserObject() );
			const float contact_limit_distance = actor_radius_data->GetRadius() + bullet_radius_data->GetRadius();

			actor_node->getChildByTag( TAG_CollisionIndicator )->setVisible( distance <= contact_limit_distance );
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
			updateDistance();
		}

		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_mole::RootScene::create() );
				return;
			}
		}
	}
}
