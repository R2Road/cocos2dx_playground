#include "step_defender_parallax_node_MoveWithCameraScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCCamera.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCParallaxNode.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

USING_NS_CC;

namespace
{
	const int TAG_Parallax = 10000;
}

namespace step_defender
{
	namespace parallax_node
	{
		MoveWithCameraScene::MoveWithCameraScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mKeyCodeCollector()
		{}

		Scene* MoveWithCameraScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) MoveWithCameraScene( back_to_the_previous_scene_callback );
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

		bool MoveWithCameraScene::init()
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
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow L/R] : Do Parralax";

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
			//
			//
			{
				const Size TotalContentSize( visibleSize.width * 2, visibleSize.height );
				setContentSize( TotalContentSize );

				auto background_root_node = ParallaxNode::create();
				background_root_node->setTag( TAG_Parallax );
				addChild( background_root_node );

				//
				// Background 1
				//
				{
					const float parallax_rate = 0.6f;
					const float part_width = 100.f;
					const float part_height = TotalContentSize.height * 0.75f;

					auto background_node = Node::create();
					background_root_node->addChild( background_node, 1, Vec2( parallax_rate, 1.f ), Vec2::ZERO );

					//
					// # Summury
					// - "... + visibleSize.width;" is need for scroll limit
					// - Background node tails meet on position x( "-TotalContentSize.width" )
					// - Do run and Show green line
					//
					const auto background_width = ( TotalContentSize.width * parallax_rate ) + visibleSize.width;
					const auto div_result = std::div( static_cast<int>( background_width ), part_width );
					Color4B current_color;
					for( int i = 0, end = div_result.quot + ( div_result.rem > 0 ? 1 : 0 ); end > i; ++i )
					{
						if( ( i & 1 ) == 0 )
						{
							current_color = Color4B::RED;
						}
						else
						{
							current_color = Color4B::BLUE;
						}

						auto layer = LayerColor::create( current_color, part_width, part_height );
						layer->setPositionX( i * part_width );
						background_node->addChild( layer );
					}

					//
					// Tail Guide
					//
					{
						auto layer = LayerColor::create( Color4B::GREEN, 5.f, part_height );
						layer->setPositionX( TotalContentSize.width * parallax_rate );
						background_node->addChild( layer, 1 );
					}
				}

				//
				// Background 2
				//
				{
					const float parallax_rate = 0.8f;
					const float part_width = 50.f;
					const float part_height = TotalContentSize.height * 0.5f;

					auto background_node = Node::create();
					background_root_node->addChild( background_node, 2, Vec2( parallax_rate, 1.f ), Vec2::ZERO );

					const auto background_width = ( TotalContentSize.width * parallax_rate ) + visibleSize.width;
					const auto div_result = std::div( static_cast<int>( background_width ), part_width );
					Color4B current_color;
					for( int i = 0, end = div_result.quot + ( div_result.rem > 0 ? 1 : 0 ); end > i; ++i )
					{
						if( ( i & 1 ) == 0 )
						{
							current_color = Color4B::YELLOW;
						}
						else
						{
							current_color = Color4B::ORANGE;
						}

						auto layer = LayerColor::create( current_color, part_width, part_height );
						layer->setPositionX( i * part_width );
						background_node->addChild( layer );
					}

					//
					// Tail Guide
					//
					{
						auto layer = LayerColor::create( Color4B::GREEN, 5.f, part_height );
						layer->setPositionX( TotalContentSize.width * parallax_rate );
						background_node->addChild( layer, 1 );
					}
				}

				//
				// Background 3
				//
				{
					const float parallax_rate = 1.f;
					const float part_width = 25.f;
					const float part_height = TotalContentSize.height * 0.25f;

					auto background_node = Node::create();
					background_root_node->addChild( background_node, 2, Vec2( parallax_rate, 1.f ), Vec2::ZERO );

					const auto background_width = ( TotalContentSize.width * parallax_rate ) + visibleSize.width;
					const auto div_result = std::div( static_cast<int>( background_width ), part_width );
					Color4B current_color;
					for( int i = 0, end = div_result.quot + ( div_result.rem > 0 ? 1 : 0 ); end > i; ++i )
					{
						if( ( i & 1 ) == 0 )
						{
							current_color = Color4B::WHITE;
						}
						else
						{
							current_color = Color4B::BLACK;
						}

						auto layer = LayerColor::create( current_color, part_width, part_height );
						layer->setPositionX( i * part_width );
						background_node->addChild( layer );
					}

					//
					// Tail Guide
					//
					{
						auto layer = LayerColor::create( Color4B::GREEN, 5.f, part_height );
						layer->setPositionX( TotalContentSize.width * parallax_rate );
						background_node->addChild( layer, 1 );
					}
				}
			}

			schedule( schedule_selector( MoveWithCameraScene::update4Move ) );

			return true;
		}

		void MoveWithCameraScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( MoveWithCameraScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( MoveWithCameraScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void MoveWithCameraScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void MoveWithCameraScene::update4Move( float delta_time )
		{
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_LEFT_ARROW ) )
			{
				const auto new_camera_position = getDefaultCamera()->getPosition() + Vec2( -150.f * delta_time, 0.f );

				CCLOG( "%f, %f", new_camera_position.x, new_camera_position.y );

				const auto new_background_position = new_camera_position - Vec2( _director->getVisibleSize().width * 0.5f, _director->getVisibleSize().height * 0.5f );
				//if( -getContentSize().width < new_background_position.x )
				{
					//auto background_node = getChildByTag( TAG_Parallax );
					//background_node->setPosition( new_background_position );

					getDefaultCamera()->setPosition( new_camera_position );
				}
			}

			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) )
			{
				const auto new_camera_position = getDefaultCamera()->getPosition() + Vec2( 150.f * delta_time, 0.f );

				CCLOG( "%f, %f", new_camera_position.x, new_camera_position.y );

				const auto new_background_position = new_camera_position - Vec2( _director->getVisibleSize().width * 0.5f, _director->getVisibleSize().height * 0.5f );
				//if( 0.f > new_background_position.x )
				{
					//auto background_node = getChildByTag( TAG_Parallax );
					//background_node->setPosition( new_background_position );

					getDefaultCamera()->setPosition( new_camera_position );
				}
			}
		}

		void MoveWithCameraScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			mKeyCodeCollector.onKeyPressed( key_code );
		}
		void MoveWithCameraScene::onKeyReleased( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			mKeyCodeCollector.onKeyReleased( key_code );
		}
	}
}
