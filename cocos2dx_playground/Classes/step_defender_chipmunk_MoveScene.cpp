#include "step_defender_chipmunk_MoveScene.h"

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
#include "physics/CCPhysicsWorld.h"

USING_NS_CC;

namespace
{
	const int TAG_MoveSpeedView = 10;
	const int TAG_PhysicsRotationView = 11;
	const int TAG_RootNode = 100;
}

namespace step_defender
{
	namespace chipmunk
	{
		MoveScene::MoveScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mKeyCodeCollector()
			, mMoveSpeed( 150 )
			, mMoveNode( nullptr )
		{}

		Scene* MoveScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) MoveScene( back_to_the_previous_scene_callback );
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

		bool MoveScene::init()
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
				ss << "[2] : Toggle Physics Rotation Enable";
				ss << std::endl;
				ss << std::endl;
				ss << "[A/S] : Move Speed Up/Down";
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

			//
			// Move Speed View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::RIGHT );
				label->setTag( TAG_MoveSpeedView );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Physics Rotation Flag View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::RIGHT );
				label->setTag( TAG_PhysicsRotationView );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height - 20.f
				) );
				addChild( label, std::numeric_limits<int>::max() );
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
							visibleOrigin.x + ( visibleSize.width * 0.25f )
							, visibleOrigin.y + ( visibleSize.height * 0.5f )
						) );
						addStaticPhysicsBody( Vec2(
							visibleOrigin.x + ( visibleSize.width * 0.75f )
							, visibleOrigin.y + ( visibleSize.height * 0.5f )
						) );
						addStaticPhysicsBody( Vec2(
							visibleOrigin.x + ( visibleSize.width * 0.5f )
							, visibleOrigin.y + ( visibleSize.height * 0.25f )
						) );
						addStaticPhysicsBody( Vec2(
							visibleOrigin.x + ( visibleSize.width * 0.5f )
							, visibleOrigin.y + ( visibleSize.height * 0.75f )
						) );
					}

					// Move Body
					{
						mMoveNode = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
						mMoveNode->setScale( _director->getContentScaleFactor() );
						{
							auto circle = PhysicsBody::createCircle( mMoveNode->getBoundingBox().size.width * 0.25f, PHYSICSBODY_MATERIAL_DEFAULT );
							circle->setDynamic( true );
							mMoveNode->setPhysicsBody( circle );
						}
						mMoveNode->setPosition( Vec2( visibleOrigin.x + ( visibleSize.width * 0.5f ), visibleOrigin.y + ( visibleSize.height * 0.5f ) ) );
						root_node->addChild( mMoveNode );
					}
				}
			}


			//
			// Setup
			//
			updateMoveSpeedView();
			updatePhysicsRotationFlagView();

			schedule( schedule_selector( MoveScene::update4Input ) );

			return true;
		}

		void MoveScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( MoveScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( MoveScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void MoveScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void MoveScene::update4Input( float delta_time )
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


		void MoveScene::addStaticPhysicsBody( const cocos2d::Vec2 sprite_position )
		{
			auto root_node = getChildByTag( TAG_RootNode );

			auto sprite = Sprite::createWithSpriteFrameName( "step_mole_target_idl_0.png" );
			sprite->setScale( _director->getContentScaleFactor() );
			{
				auto circle = PhysicsBody::createCircle( sprite->getBoundingBox().size.width * 0.25f, PHYSICSBODY_MATERIAL_DEFAULT );
				circle->setDynamic( false );
				sprite->setPhysicsBody( circle );
			}
			sprite->setPosition( sprite_position );
			root_node->addChild( sprite );
		}

		void MoveScene::updateMoveSpeedView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_MoveSpeedView ) );
			label->setString( StringUtils::format( "+ Speed : %d", mMoveSpeed ) );
		}
		void MoveScene::updatePhysicsRotationFlagView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_PhysicsRotationView ) );
			label->setString( StringUtils::format( "+ Physics Rotation : %s", ( mMoveNode->getPhysicsBody()->isRotationEnabled() ? "ON" : "OFF" ) ) );
		}

		void MoveScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
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

			//
			// Physics Rotation Flag View
			//
			if( EventKeyboard::KeyCode::KEY_2 == key_code )
			{
				auto new_flag = !mMoveNode->getPhysicsBody()->isRotationEnabled();
				mMoveNode->getPhysicsBody()->setRotationEnable( new_flag );

				if( !new_flag )
				{
					mMoveNode->getPhysicsBody()->setAngularVelocity( 0.f );
				}

				updatePhysicsRotationFlagView();
			}

			if( EventKeyboard::KeyCode::KEY_A == key_code )
			{
				mMoveSpeed += 25;
				updateMoveSpeedView();
			}
			if( EventKeyboard::KeyCode::KEY_S == key_code )
			{
				mMoveSpeed = std::max( 0, mMoveSpeed - 25 );
				updateMoveSpeedView();
			}

			mKeyCodeCollector.onKeyPressed( key_code );
		}
		void MoveScene::onKeyReleased( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event )
		{
			mKeyCodeCollector.onKeyReleased( key_code );
		}
	}
}
