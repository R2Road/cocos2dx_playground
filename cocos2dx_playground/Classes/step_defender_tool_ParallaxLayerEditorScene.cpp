#include "step_defender_tool_ParallaxLayerEditorScene.h"

#include <functional>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCParallaxNode.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"

#include "cpgui_ToolBarNode.h"

#include "step_defender_game_Constant.h"

USING_NS_CC;

namespace
{
	const float ScrollSpeed = 300.f;

	const Color3B BackgroundColors[3] = {
		{ 90u, 200u, 255u }
		, Color3B::YELLOW
		, Color3B::WHITE
	};

	const int Parallax_Ruler_Part_Width = 100.f;
}

namespace step_defender
{
	namespace tool
	{
		ParallaxLayerEditorScene::ParallaxLayerEditorScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mKeyCodeCollector()

			, mParallaxRulerNode( nullptr )
			, mTouchNode( nullptr )
			, mStage( nullptr )
		{}

		Scene* ParallaxLayerEditorScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) ParallaxLayerEditorScene( back_to_the_previous_scene_callback );
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

		bool ParallaxLayerEditorScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleOrigin = _director->getVisibleOrigin();
			const auto visibleSize = _director->getVisibleSize();
			const Vec2 visibleCenter(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			);

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
			// Sky
			//
			{
				auto layer = LayerColor::create( game::WorldConfig.SkyColor );
				addChild( layer, std::numeric_limits<int>::min() );
			}

			//
			// Layer Selector
			//
			{
				auto tool_bar_node = cpgui::ToolBarNode::create();
				addChild( tool_bar_node, std::numeric_limits<int>::max() );

				tool_bar_node->AddTool( 1, "1", 10, std::bind( &ParallaxLayerEditorScene::onLayerSelect, this, 0 ) );
				tool_bar_node->AddTool( 2, "2", 10, std::bind( &ParallaxLayerEditorScene::onLayerSelect, this, 1 ) );
				tool_bar_node->AddTool( 3, "3", 10, std::bind( &ParallaxLayerEditorScene::onLayerSelect, this, 2 ) );

				tool_bar_node->setPosition(
					visibleCenter.x - ( tool_bar_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + visibleSize.height - tool_bar_node->getContentSize().height
				);

				// Set Indicator
				tool_bar_node->SelectTool( 1 );
			}

			//
			// Touch Node
			//
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setAnchorPoint( Vec2::ZERO );
				button->setScale9Enabled( true );
				button->setContentSize( game::WorldConfig.WorldSize + Size( +visibleSize.width, 0.f ) );
				addChild( button, std::numeric_limits<int>::max() - 1 );

				mTouchNode = button;
			}

			struct ParallaxNodeConfig
			{
				int Index = -1;
				float Rate = 1.f;
				float RulerY = 0.f;
			};
			const ParallaxNodeConfig ParallaxNodeConfigContainer[3] = {
				{ 0, 0.6f, 0.2f }
				, { 1, 0.8f, 0.25f }
				, { 2, 1.f, 0.3f }
			};

			//
			// ParallaxNode Setup - Ruler
			//
			{
				setContentSize( game::WorldConfig.WorldSize );

				mParallaxRulerNode = ParallaxNode::create();
				addChild( mParallaxRulerNode, 0 );

				for( const auto& c : ParallaxNodeConfigContainer )
				{
					auto background_node = Node::create();
					background_node->setTag( c.Index );
					background_node->setCascadeOpacityEnabled( true );
					mParallaxRulerNode->addChild( background_node, c.Index, Vec2( c.Rate, 1.f ), Vec2::ZERO );

					const auto background_width = ( game::WorldConfig.WorldSize.width * c.Rate ) + visibleSize.width;
					const auto div_result = std::div( static_cast<int>( background_width ), Parallax_Ruler_Part_Width );
					for( int i = 0, end = div_result.quot + ( div_result.rem > 0 ? 1 : 0 ); end > i; ++i )
					{
						auto label = Label::createWithTTF( std::to_string( i * Parallax_Ruler_Part_Width ), "fonts/NanumSquareR.ttf", 6 + c.Index, Size::ZERO, TextHAlignment::LEFT );
						label->setAnchorPoint( Vec2( 0.f, 1.f ) );
						label->setColor( BackgroundColors[c.Index] );
						label->setPosition( Vec2(
							i * Parallax_Ruler_Part_Width
							, game::WorldConfig.WorldSize.height - ( game::WorldConfig.WorldSize.height * c.RulerY )
						) );
						background_node->addChild( label, std::numeric_limits<int>::max() );
					}

					//
					// Tail Guide
					//
					{
						auto layer = LayerColor::create( Color4B::GREEN, 5.f, game::WorldConfig.WorldSize.height * c.RulerY );
						layer->setPositionX( game::WorldConfig.WorldSize.width * c.Rate );
						background_node->addChild( layer, 1 );
					}
				}
			}

			//
			// Stage Area
			//
			{
				mStage = LayerColor::create( Color4B::GRAY, game::WorldConfig.BottomSize.width, game::WorldConfig.BottomSize.height );
				addChild( mStage, 1 );
			}

			//
			// Setup
			//
			schedule( schedule_selector( ParallaxLayerEditorScene::update4Move ) );
			onLayerSelect( 0 );

			return true;
		}

		void ParallaxLayerEditorScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ParallaxLayerEditorScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( ParallaxLayerEditorScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ParallaxLayerEditorScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void ParallaxLayerEditorScene::update4Move( float delta_time )
		{
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) )
			{
				const auto new_position = mParallaxRulerNode->getPositionX() + ( -ScrollSpeed * delta_time );
				if( -getContentSize().width < new_position )
				{
					mParallaxRulerNode->setPositionX( new_position );
					mTouchNode->setPositionX( new_position );
				}
				else
				{
					mParallaxRulerNode->setPositionX( -getContentSize().width );
					mTouchNode->setPositionX( -getContentSize().width );
				}

				mStage->setPosition( mParallaxRulerNode->getPosition() );
			}

			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_LEFT_ARROW ) )
			{
				const auto new_position = mParallaxRulerNode->getPositionX() + ( ScrollSpeed * delta_time );
				if( 0.f > new_position )
				{
					mParallaxRulerNode->setPositionX( new_position );
					mTouchNode->setPositionX( new_position );
				}
				else
				{
					mParallaxRulerNode->setPositionX( 0.f );
					mTouchNode->setPositionX( 0.f );
				}

				mStage->setPosition( mParallaxRulerNode->getPosition() );
			}
		}


		void ParallaxLayerEditorScene::onLayerSelect( const int layer_index )
		{
			for( auto c : mParallaxRulerNode->getChildren() )
			{
				if( layer_index == c->getTag() )
				{
					mCurrentBackgroundLayer = c;
					c->setOpacity( 255u );
				}
				else
				{
					c->setOpacity( 80u );
				}
			}
			mTouchNode->setColor( BackgroundColors[layer_index] );
		}


		void ParallaxLayerEditorScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
			mKeyCodeCollector.onKeyPressed( key_code );
		}
		void ParallaxLayerEditorScene::onKeyReleased( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			mKeyCodeCollector.onKeyReleased( key_code );
		}
	}
}
