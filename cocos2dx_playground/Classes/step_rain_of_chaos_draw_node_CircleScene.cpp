#include "step_rain_of_chaos_draw_node_CircleScene.h"

#include <new>
#include <numeric>

#include "2d/CCDrawNode.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace draw_node
	{
		CircleScene::CircleScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* CircleScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) CircleScene( back_to_the_previous_scene_callback );
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

		bool CircleScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

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
				auto background_layer = LayerColor::create( Color4B( 130, 49, 29, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Draw Circle 1
			//
			{
				const Vec2 draw_node_position( visibleOrigin.x + visibleSize.width * 0.2f, visibleOrigin.y + visibleSize.height * 0.4f );
				const float radius = 30.f;
				const float angle = 0.f;
				const unsigned int segments = 10;
				const bool draw_line2center = true;

				auto draw_node = cocos2d::DrawNode::create();
				draw_node->drawCircle(
					draw_node_position
					, radius, angle
					, segments
					, draw_line2center
					, 1.0f, 1.0f
					, Color4F::WHITE
				);
				addChild( draw_node, std::numeric_limits<int>::min() );

				// Info
				{
					auto label = Label::createWithTTF(
						StringUtils::format( "radius = %.2f\n angle = %.2f\n segments = %d\n draw_line2center = %s", radius, angle, segments, ( draw_line2center ? "true" : "false" ) )
						, cpg::StringTable::GetFontPath()
						, 8, Size::ZERO, TextHAlignment::RIGHT
					);
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						draw_node_position + Vec2( 0.f, 80.f )
					);
					addChild( label );
				}
			}

			//
			// Draw Circle 2
			//
			{
				const Vec2 draw_node_position( visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.4f );
				const float radius = 50.f;
				const float angle = 0.f;
				const unsigned int segments = 50;
				const bool draw_line2center = false;

				auto draw_node = cocos2d::DrawNode::create();
				draw_node->drawCircle(
					draw_node_position
					, radius, angle
					, segments
					, draw_line2center
					, 1.0f, 1.0f
					, Color4F::WHITE
				);
				addChild( draw_node, std::numeric_limits<int>::min() );

				// Info
				{
					auto label = Label::createWithTTF(
						StringUtils::format( "radius = %.2f\n angle = %.2f\n segments = %d\n draw_line2center = %s", radius, angle, segments, ( draw_line2center ? "true" : "false" ) )
						, cpg::StringTable::GetFontPath()
						, 8, Size::ZERO, TextHAlignment::RIGHT
					);
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						draw_node_position + Vec2( 0.f, 80.f )
					);
					addChild( label );
				}
			}

			//
			// Draw Circle 3
			//
			{
				const Vec2 draw_node_position( visibleOrigin.x + visibleSize.width * 0.8f, visibleOrigin.y + visibleSize.height * 0.4f );
				const float radius = 40.f;
				const float angle = 0.1f;
				const unsigned int segments = 50;
				const bool draw_line2center = true;

				auto draw_node = cocos2d::DrawNode::create();
				draw_node->drawCircle(
					draw_node_position
					, radius, angle
					, segments
					, draw_line2center
					, 1.0f, 1.0f
					, Color4F::WHITE
				);
				addChild( draw_node, std::numeric_limits<int>::min() );

				// Info
				{
					auto label = Label::createWithTTF(
						StringUtils::format( "radius = %.2f\n angle = %.2f\n segments = %d\n draw_line2center = %s", radius, angle, segments, ( draw_line2center ? "true" : "false" ) )
						, cpg::StringTable::GetFontPath()
						, 8, Size::ZERO, TextHAlignment::RIGHT
					);
					label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						draw_node_position + Vec2( 0.f, 80.f )
					);
					addChild( label );
				}
			}

			return true;
		}

		void CircleScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( CircleScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void CircleScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void CircleScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
