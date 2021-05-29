#include "step_defender_chipmunk_BasicScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "physics/CCPhysicsWorld.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const int TAG_RootNode = 100;
}

namespace step_defender
{
	namespace chipmunk
	{
		BasicScene::BasicScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
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
			//
			// Physics Setup
			//
			if( !Scene::initWithPhysics() )
			{
				return false;
			}
			getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );

			const auto visibleOrigin = _director->getVisibleOrigin();
			const auto visibleSize = _director->getVisibleSize();

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
				ss << "[1] : Toggle Physics Debug Draw";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[SPACE] : Add Physics Body";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
						addPhysicsBody(
							Vec2( visibleOrigin.x + ( visibleSize.width * 0.25f ), visibleOrigin.y + ( visibleSize.height * 0.5f ) )
							, false
						);
						addPhysicsBody(
							Vec2( visibleOrigin.x + ( visibleSize.width * 0.75f ), visibleOrigin.y + ( visibleSize.height * 0.5f ) )
							, false
						);
					}

					// Dynamic Body
					{
						addPhysicsBody(
							Vec2( visibleOrigin.x + ( visibleSize.width * 0.5f ), visibleOrigin.y + ( visibleSize.height * 0.5f ) )
							, true
						);
					}
				}
			}

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


		void BasicScene::addPhysicsBody( const cocos2d::Vec2 sprite_position, const bool is_dynamic )
		{
			auto root_node = getChildByTag( TAG_RootNode );

			auto sprite = Sprite::createWithSpriteFrameName( is_dynamic ? "actor001_run_01.png" : "step_mole_target_idl_0.png" );
			sprite->setScale( _director->getContentScaleFactor() );
			{
				auto circle = PhysicsBody::createCircle( sprite->getBoundingBox().size.width * 0.25f, PHYSICSBODY_MATERIAL_DEFAULT );
				circle->setDynamic( is_dynamic );
				sprite->setPhysicsBody( circle );
			}
			sprite->setPosition( sprite_position );
			root_node->addChild( sprite );
		}

		void BasicScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
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
			// Add Physics Body
			//
			if( EventKeyboard::KeyCode::KEY_SPACE == key_code )
			{
				auto root_node = getChildByTag( TAG_RootNode );
				addPhysicsBody( Vec2( 0.f, root_node->getContentSize().height ), true );
			}
		}
	}
}
