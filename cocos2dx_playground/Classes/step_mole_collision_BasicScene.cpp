#include "step_mole_collision_BasicScene.h"

#include <new>
#include <numeric>

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

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const int TAG_CollisionIndicator = 20140419;

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
		BasicScene::BasicScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mActorNode( nullptr )
			, mBulletNode( nullptr )
			, mDistanceLabel( nullptr )
		{}

		Scene* BasicScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) BasicScene( back_to_the_previous_scene_callback );
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
			// Distance
			//
			{
				mDistanceLabel = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12 );
				mDistanceLabel->setColor( Color3B::GREEN );
				mDistanceLabel->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				mDistanceLabel->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleMargin.height )
				);
				addChild( mDistanceLabel, std::numeric_limits<int>::max() );
			}

			//
			// Touch Pannel
			//
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->setContentSize( visibleSize );
				button->setOpacity( 150u );
				button->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				button->addTouchEventListener( CC_CALLBACK_2( BasicScene::onButton, this ) );
				addChild( button );
			}

			//
			// Actor
			//
			{
				mActorNode = Node::create();
				mActorNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.3f )
				);
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

					// Radius View
					{
						auto label = Label::createWithTTF( StringUtils::format( "%.2f", radius ), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
						label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
						label->setPositionX( radius + margin.width );
						mActorNode->addChild( label );
					}

					// Collision Guide
					{
						auto guide = Sprite::createWithSpriteFrameName( "guide_02_4.png" );
						guide->setScale( radius / ( guide->getContentSize().width * 0.5f ) );
						mActorNode->addChild( guide );
					}

					// Collision Indicator
					{
						auto collision_indicator_node = Sprite::createWithSpriteFrameName( "guide_02_7.png" );
						collision_indicator_node->setTag( TAG_CollisionIndicator );
						collision_indicator_node->setScale( radius / ( collision_indicator_node->getContentSize().width * 0.5f ) );
						collision_indicator_node->setVisible( false );
						mActorNode->addChild( collision_indicator_node );
					}

					// Radius Data
					{
						auto radius_data( new RadiusData( radius ) );
						mActorNode->setUserObject( radius_data );
						radius_data->release();
					}
				}
				addChild( mActorNode, Z_Actor );
			}

			//
			// Bullet
			//
			{
				mBulletNode = Node::create();
				mBulletNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.7f )
				);
				{
					// Pivot
					{
						auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
						pivot->setScale( 4.f );
						mBulletNode->addChild( pivot, std::numeric_limits<int>::max() );
					}

					// View
					auto view_node = Sprite::createWithSpriteFrameName( "step_mole_target_wait_0.png" );
					view_node->setScale( _director->getContentScaleFactor() );
					mBulletNode->addChild( view_node );
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
						mBulletNode->addChild( label );
					}

					// Collision Guide
					{
						auto guide = Sprite::createWithSpriteFrameName( "guide_02_4.png" );
						guide->setScale( radius / ( guide->getContentSize().width * 0.5f ) );
						mBulletNode->addChild( guide );
					}

					// Radius Data
					{
						auto radius_data( new RadiusData( radius ) );
						mBulletNode->setUserObject( radius_data );
						radius_data->release();
					}
				}
				addChild( mBulletNode, Z_Bullet );
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
			const float distance = mActorNode->getPosition().distance( mBulletNode->getPosition() );

			const auto actor_radius_data = static_cast<RadiusData*>( mActorNode->getUserObject() );
			const auto bullet_radius_data = static_cast<RadiusData*>( mBulletNode->getUserObject() );
			const float contact_limit_distance = actor_radius_data->GetRadius() + bullet_radius_data->GetRadius();

			mActorNode->getChildByTag( TAG_CollisionIndicator )->setVisible( distance <= contact_limit_distance );
		}
		void BasicScene::updateDistance()
		{
			const auto distance = mBulletNode->getPosition().distance( mActorNode->getPosition() );

			mDistanceLabel->setString( StringUtils::format( "Distance : %.2f", distance ) );
		}

		void BasicScene::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
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
			updateDistance();
		}

		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
