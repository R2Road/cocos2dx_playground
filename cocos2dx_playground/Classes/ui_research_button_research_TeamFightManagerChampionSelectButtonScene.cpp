#include "ui_research_button_research_TeamFightManagerChampionSelectButtonScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCDrawNode.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "ui/UILayout.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_node_PivotNode.h"
#include "cpg_ui_EXButtonNode.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace ui_research
{
	namespace button_research
	{
		TeamFightManagerChampionSelectButtonScene::TeamFightManagerChampionSelectButtonScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* TeamFightManagerChampionSelectButtonScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) TeamFightManagerChampionSelectButtonScene( back_to_the_previous_scene_callback );
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

		bool TeamFightManagerChampionSelectButtonScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();
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
				auto background_layer = LayerColor::create( Color4B( 50, 75, 112, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Research x 1
			//
			{
				Vec2 points[3] = { Vec2::ZERO, Vec2( 0.f, 60.f ), Vec2( 30.f, 60.f ) };

				auto draw_node = DrawNode::create();
				draw_node->drawPolygon( points, 3, Color4F::GREEN, 0.f, Color4F::GREEN );
				draw_node->setPosition( visibleSize.width * 0.2f, visibleCenter.y );
				addChild( draw_node );

				// Pivot
				{
					auto pivot_node = cpg_node::PivotNode::create();
					draw_node->addChild( pivot_node, std::numeric_limits<int>::max() );
				}

				{
					auto rotate_action = RotateBy::create( 1.f, -360.f );
					auto repeat_action = RepeatForever::create( rotate_action );
					draw_node->runAction( repeat_action );
				}
			}

			//
			// Research x 2
			//
			{
				auto layout = ui::Layout::create();
				layout->setContentSize( Size( 100.f, 100.f ) );
				layout->setBackGroundColorType( ui::Layout::BackGroundColorType::SOLID );
				layout->setBackGroundColor( Color3B::GRAY );
				layout->setClippingEnabled( true );
				layout->setPosition(
					visibleOrigin
					+ visibleCenter
					- Vec2( layout->getContentSize().width * 0.5f, layout->getContentSize().height * 0.5f )
				);
				addChild( layout );

				// Pivot
				{
					auto pivot_node = cpg_node::PivotNode::create();
					layout->addChild( pivot_node, std::numeric_limits<int>::max() );
				}

				// Polygon
				{
					const float required_height = std::sqrt( pow( 50.f, 2 ) + pow( 50.f, 2) );

					Vec2 points[3] = { Vec2::ZERO, Vec2( -required_height, required_height ), Vec2( 0.f, required_height ) };

					auto draw_node = DrawNode::create();
					draw_node->drawPolygon( points, 3, Color4F::WHITE, 0.f, Color4F::WHITE );
					draw_node->setPosition( Vec2( layout->getContentSize().width * 0.5f, layout->getContentSize().height * 0.5f ) );
					layout->addChild( draw_node, 0 );

					{
						auto rotate_action = RotateBy::create( 3.f, -360.f );
						auto repeat_action = RepeatForever::create( rotate_action );
						draw_node->runAction( repeat_action );

					}
				}

				// Layer
				{
					auto layer = LayerColor::create( Color4B::BLACK, 98, 98 );
					layer->setVisible( true );
					layer->setPosition( 1.f, 1.f );
					layout->addChild( layer, 1 );
				}
			}

			//
			// Research x 3
			//
			{
				const Size button_size( 100.f, 100.f );

				auto ex_button = cpg_ui::EXButtonNode::create( button_size );
				ex_button->setPosition( Vec2( visibleSize.width * 0.75f, visibleCenter.y ) );
				addChild( ex_button );
				
				ex_button->SetBackground( LayerColor::create( Color4B::BLACK, 100u, 100u ) );

				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_0.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( button_size );

					ex_button->SetView( cpg_ui::EXButtonNode::eViewIndex::Normal, sprite );
				}

				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_1.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( button_size );
					sprite->setVisible( false );
					{
						auto label = Label::createWithTTF( "EX Button", cpg::StringTable::GetFontPath(), 10 );
						label->setPosition( sprite->getContentSize().width * 0.5f, sprite->getContentSize().height * 0.5f );
						{
							auto fadeOutAction = FadeOut::create( 0.8f );
							auto fadeOutkDelay = DelayTime::create( 0.2f );
							auto fadeInAction = FadeIn::create( 0.6f );
							auto fadeInkDelay = DelayTime::create( 0.4f );
							auto blinkSequence = Sequence::create( fadeOutAction, fadeOutkDelay, fadeInAction, fadeInkDelay, nullptr );
							auto blinkrepeat = RepeatForever::create( blinkSequence );
							label->runAction( blinkrepeat );
						}
						sprite->addChild( label );
					}

					ex_button->SetView( cpg_ui::EXButtonNode::eViewIndex::MouseOver, sprite );
				}

				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_2.png" );
					sprite->setVisible( false );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( button_size );

					ex_button->SetView( cpg_ui::EXButtonNode::eViewIndex::Push, sprite );
				}

				ex_button->SetCallback( []( const cpg_ui::EXButtonNode::eButtonEvent button_event )
				{
					switch( button_event )
					{
					case cpg_ui::EXButtonNode::eButtonEvent::MouseOver:
						CCLOG( "MouseOver" );
						break;
					case cpg_ui::EXButtonNode::eButtonEvent::MouseLeave:
						CCLOG( "MouseLeave" );
						break;
					case cpg_ui::EXButtonNode::eButtonEvent::Push:
						CCLOG( "Push" );
						break;
					case cpg_ui::EXButtonNode::eButtonEvent::Move:
						CCLOG( "Move" );
						break;
					case cpg_ui::EXButtonNode::eButtonEvent::Release:
						CCLOG( "Release" );
						break;
					}
				} );
			}

			return true;
		}

		void TeamFightManagerChampionSelectButtonScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TeamFightManagerChampionSelectButtonScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void TeamFightManagerChampionSelectButtonScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void TeamFightManagerChampionSelectButtonScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
