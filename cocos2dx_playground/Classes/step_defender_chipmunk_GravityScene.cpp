#include "step_defender_chipmunk_GravityScene.h"

#include <new>
#include <numeric>

#include "2d/CCDrawNode.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/UIButton.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const int TAG_GravityView = 10;
	const int TAG_GravityPivot = 11;
	const int TAG_GravityGuide = 12;
	const int TAG_RootNode = 100;
}

namespace step_defender
{
	namespace chipmunk
	{
		GravityScene::GravityScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mGravityHelperButton_MoveOffset()
		{}

		Scene* GravityScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) GravityScene( back_to_the_previous_scene_callback );
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

		bool GravityScene::init()
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
			const Vec2 visibleCenter( visibleOrigin.x + ( visibleSize.width * 0.5f ), visibleOrigin.y + ( visibleSize.height * 0.5f ) );

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
			// Gravity View
			//
			{
				auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::RIGHT );
				label->setTag( TAG_GravityView );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateGravityView();
			}

			//
			// Gravity Helper
			//
			{
				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setTag( TAG_GravityPivot );
					pivot->setScale( _director->getContentScaleFactor() );
					pivot->setPosition( visibleCenter );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				// Helper
				{
					auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
					button->setPosition( visibleCenter );
					button->addTouchEventListener( CC_CALLBACK_2( GravityScene::onGravityHelperButton, this ) );
					addChild( button, std::numeric_limits<int>::max() - 1 );

					// left label
					{
						auto label = Label::createWithTTF( "CLICK HERE ===>>>", cpg::StringTable::GetFontPath(), 10 );
						label->setColor( Color3B::GREEN );
						label->setAnchorPoint( Vec2( 1.f, 0.5f ) );
						label->setPosition(
							-4.f
							, button->getContentSize().height * 0.5f
						);
						button->addChild( label );
					}

					// right label
					{
						auto label = Label::createWithTTF( "<<<=== ...AND DRAG", cpg::StringTable::GetFontPath(), 10 );
						label->setColor( Color3B::GREEN );
						label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
						label->setPosition(
							button->getContentSize().width + 4.f
							, button->getContentSize().height * 0.5f
						);
						button->addChild( label );
					}
				}

				// Guide
				{
					auto draw_node = cocos2d::DrawNode::create();
					draw_node->setTag( TAG_GravityGuide );
					draw_node->drawLine(
						visibleCenter
						, visibleCenter
						, Color4F::GREEN
					);
					addChild( draw_node, std::numeric_limits<int>::max() - 1 );
				}
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
							Vec2( visibleOrigin.x + ( visibleSize.width * 0.25f ), visibleOrigin.y + ( visibleSize.height * 0.25f ) )
							, false
						);
						addPhysicsBody(
							Vec2( visibleOrigin.x + ( visibleSize.width * 0.75f ), visibleOrigin.y + ( visibleSize.height * 0.25f ) )
							, false
						);
						addPhysicsBody(
							Vec2( visibleOrigin.x + ( visibleSize.width * 0.5f ), visibleOrigin.y + ( visibleSize.height * 0.75f ) )
							, false
						);
					}

					// Dynamic Body
					{
						addPhysicsBody(
							visibleCenter
							, true
						);
					}
				}
			}

			return true;
		}

		void GravityScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GravityScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void GravityScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void GravityScene::addPhysicsBody( const cocos2d::Vec2 sprite_position, const bool is_dynamic )
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

		void GravityScene::updateGravityView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_GravityView ) );
			label->setString( StringUtils::format( "+ Gravity\nx : %.1f, y : %.1f", getPhysicsWorld()->getGravity().x, getPhysicsWorld()->getGravity().y ) );
		}

		void GravityScene::onGravityHelperButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				mGravityHelperButton_MoveOffset = button->getPosition() - button->getTouchBeganPosition();
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				button->setPosition( button->getTouchMovePosition() + mGravityHelperButton_MoveOffset );

				//
				// Update Gravity
				//
				const auto pivot_position = getChildByTag( TAG_GravityPivot )->getPosition();
				const auto gravity_direction = button->getPosition() - pivot_position;
				getPhysicsWorld()->setGravity( gravity_direction * 2.f );

				//
				// Update Gravity Guide
				//
				{
					auto draw_node = static_cast<DrawNode*>( getChildByTag( TAG_GravityGuide ) );
					draw_node->clear();
					draw_node->drawLine( pivot_position, pivot_position + gravity_direction, Color4F::GREEN );
				}

				//
				// Update Gravity View
				//
				updateGravityView();
			}
		}

		void GravityScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
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
