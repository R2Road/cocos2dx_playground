#include "step_defender_chipmunk_ContactScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"
#include "physics/CCPhysicsContact.h"
#include "physics/CCPhysicsWorld.h"

USING_NS_CC;

namespace
{
	const int TAG_MoveSpeedView = 10;
	const int TAG_RootNode = 100;
}

namespace step_defender
{
	namespace chipmunk
	{
		ContactScene::ContactScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mKeyCodeCollector()

			, mEventListenerPhysicsContact( nullptr )

			, mMoveSpeed( 150 )
			, mMoveNode( nullptr )
		{}

		Scene* ContactScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) ContactScene( back_to_the_previous_scene_callback );
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

		bool ContactScene::init()
		{
			//
			// Physics Setup
			//
			if( !Scene::initWithPhysics() )
			{
				return false;
			}
			getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );
			getPhysicsWorld()->setGravity( Vec2::ZERO );

			const auto visibleOrigin = _director->getVisibleOrigin();
			const auto visibleSize = _director->getVisibleSize();

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
				ss << "[1] : Toggle Physics Debug Draw";
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow L/R/U/D] : Move";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}
			
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 7, 39, 43, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			{
				
			}

			//
			// Test Setup
			//
			{
				auto root_node = Node::create();
				root_node->setTag( TAG_RootNode );
				root_node->setContentSize( visibleSize );
				root_node->setPosition( visibleOrigin.x, visibleOrigin.y );
				addChild( root_node );

				//
				// Guide
				//
				{
					auto edge_box_component = PhysicsBody::createEdgeBox(
						visibleSize
						, PHYSICSBODY_MATERIAL_DEFAULT
						, 2.f
					);
					root_node->setPhysicsBody( edge_box_component );
				}
				
				//
				// Bodies
				//
				{
					// Static Body
					{
						addStaticPhysicsBody( Vec2(
							visibleOrigin.x + ( visibleSize.width * 0.45f )
							, visibleOrigin.y + ( visibleSize.height * 0.25f )
						) );
						addStaticPhysicsBody( Vec2(
							visibleOrigin.x + ( visibleSize.width * 0.55f )
							, visibleOrigin.y + ( visibleSize.height * 0.25f )
						) );

						// Explain Label
						{
							auto label = Label::createWithTTF( "STATIC BODY ===>>>", "fonts/NanumSquareR.ttf", 10 );
							label->setColor( Color3B::GREEN );
							label->setAnchorPoint( Vec2( 1.f, 0.5f ) );
							label->setPosition(
								visibleOrigin.x + ( visibleSize.width * 0.4f )
								, visibleOrigin.y + ( visibleSize.height * 0.25f )
							);
							addChild( label );
						}
					}

					// Static Sensor
					{
						addStaticPhysicsSensor( Vec2(
							visibleOrigin.x + ( visibleSize.width * 0.45f )
							, visibleOrigin.y + ( visibleSize.height * 0.75f )
						) );
						addStaticPhysicsSensor( Vec2(
							visibleOrigin.x + ( visibleSize.width * 0.5f )
							, visibleOrigin.y + ( visibleSize.height * 0.75f )
						) );
						addStaticPhysicsSensor( Vec2(
							visibleOrigin.x + ( visibleSize.width * 0.55f )
							, visibleOrigin.y + ( visibleSize.height * 0.75f )
						) );

						// Explain Label
						{
							auto label = Label::createWithTTF( "<<<=== SENSOR", "fonts/NanumSquareR.ttf", 10 );
							label->setColor( Color3B::GREEN );
							label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
							label->setPosition(
								visibleOrigin.x + ( visibleSize.width * 0.6f )
								, visibleOrigin.y + ( visibleSize.height * 0.75f )
							);
							addChild( label );
						}
					}

					// Move Body
					{
						mMoveNode = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
						mMoveNode->setScale( _director->getContentScaleFactor() );
						{
							auto circle = PhysicsBody::createCircle( mMoveNode->getBoundingBox().size.width * 0.25f, PHYSICSBODY_MATERIAL_DEFAULT );
							circle->setDynamic( true );
							circle->setContactTestBitmask( true );
							mMoveNode->setPhysicsBody( circle );
						}
						mMoveNode->setPosition( Vec2( visibleOrigin.x + ( visibleSize.width * 0.5f ), visibleOrigin.y + ( visibleSize.height * 0.5f ) ) );
						root_node->addChild( mMoveNode );
					}
				}
			}

			schedule( schedule_selector( ContactScene::update4Input ) );

			return true;
		}

		void ContactScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ContactScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( ContactScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );

			assert( !mEventListenerPhysicsContact );
			mEventListenerPhysicsContact = EventListenerPhysicsContact::create();
			mEventListenerPhysicsContact->onContactBegin = CC_CALLBACK_1( ContactScene::onContactBegin, this );
			mEventListenerPhysicsContact->onContactPreSolve = CC_CALLBACK_2( ContactScene::onContactPreSolve, this );
			mEventListenerPhysicsContact->onContactPostSolve = CC_CALLBACK_2( ContactScene::onContactPostSolve, this );
			mEventListenerPhysicsContact->onContactSeparate = CC_CALLBACK_1( ContactScene::onContactSeparate, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mEventListenerPhysicsContact, this );
		}
		void ContactScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			assert( mEventListenerPhysicsContact );
			getEventDispatcher()->removeEventListener( mEventListenerPhysicsContact );
			mEventListenerPhysicsContact = nullptr;

			Scene::onExit();
		}


		void ContactScene::update4Input( float delta_time )
		{
			Vec2 velocity;
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_UP_ARROW ) )
			{
				velocity.y += mMoveSpeed;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_DOWN_ARROW ) )
			{
				velocity.y -= mMoveSpeed;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) )
			{
				velocity.x += mMoveSpeed;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_LEFT_ARROW ) )
			{
				velocity.x -= mMoveSpeed;
			}

			mMoveNode->getPhysicsBody()->setVelocity( velocity );
		}


		bool ContactScene::onContactBegin( PhysicsContact& contact )
		{
			CCLOG( "onContactBegin" );

			return true;
		}
		bool ContactScene::onContactPreSolve( PhysicsContact& contact, PhysicsContactPreSolve& solve )
		{
			CCLOG( "onContactPreSolve" );
			return true;
		}
		void ContactScene::onContactPostSolve( PhysicsContact& contact, const PhysicsContactPostSolve& solve )
		{
			CCLOG( "onContactPostSolve" );
		}
		void ContactScene::onContactSeparate( PhysicsContact& contact )
		{
			CCLOG( "onContactSeparate" );
		}


		void ContactScene::addStaticPhysicsBody( const cocos2d::Vec2 sprite_position )
		{
			auto root_node = getChildByTag( TAG_RootNode );

			auto sprite = Sprite::createWithSpriteFrameName( "step_mole_target_idl_0.png" );
			sprite->setScale( _director->getContentScaleFactor() );
			{
				auto circle = PhysicsBody::createCircle( sprite->getBoundingBox().size.width * 0.25f, PHYSICSBODY_MATERIAL_DEFAULT );
				circle->setDynamic( false );
				circle->setContactTestBitmask( true );
				sprite->setPhysicsBody( circle );
			}
			sprite->setPosition( sprite_position );
			root_node->addChild( sprite );
		}

		void ContactScene::addStaticPhysicsSensor( const cocos2d::Vec2 sprite_position )
		{
			auto root_node = getChildByTag( TAG_RootNode );

			auto sprite = Sprite::createWithSpriteFrameName( "step_mole_target_dmgd1_0.png" );
			sprite->setScale( _director->getContentScaleFactor() );
			{
				auto circle = PhysicsBody::createCircle( sprite->getBoundingBox().size.width * 0.25f, PHYSICSBODY_MATERIAL_DEFAULT );
				circle->setDynamic( false );
				circle->setContactTestBitmask( true );
				circle->setCollisionBitmask( 0 );
				sprite->setPhysicsBody( circle );
			}
			sprite->setPosition( sprite_position );
			root_node->addChild( sprite );
		}


		void ContactScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			//
			// Debug View
			//
			if( EventKeyboard::KeyCode::KEY_1 == key_code )
			{
				getPhysicsWorld()->setDebugDrawMask(
					PhysicsWorld::DEBUGDRAW_NONE == getPhysicsWorld()->getDebugDrawMask()
					? PhysicsWorld::DEBUGDRAW_ALL
					: PhysicsWorld::DEBUGDRAW_NONE
				);
			}

			mKeyCodeCollector.onKeyPressed( key_code );
		}
		void ContactScene::onKeyReleased( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event )
		{
			mKeyCodeCollector.onKeyReleased( key_code );
		}
	}
}
