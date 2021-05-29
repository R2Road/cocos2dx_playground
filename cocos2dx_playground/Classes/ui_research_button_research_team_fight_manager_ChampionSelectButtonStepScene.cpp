#include "ui_research_button_research_team_fight_manager_ChampionSelectButtonStepScene.h"

#include <new>
#include <numeric>

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

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace ui_research
{
	namespace button_research
	{
		namespace team_fight_manager
		{
			ChampionSelectButtonStepScene::ChampionSelectButtonStepScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
				helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
				, mKeyboardListener( nullptr )
			{}

			Scene* ChampionSelectButtonStepScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
			{
				auto ret = new ( std::nothrow ) ChampionSelectButtonStepScene( back_to_the_previous_scene_callback );
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

			bool ChampionSelectButtonStepScene::init()
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
					ss << cpg::linefeed;
					ss << cpg::linefeed;
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
					draw_node->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.2f, visibleSize.height * 0.5f )
					);
					addChild( draw_node );

					// Pivot
					{
						auto pivot_node = cpg_node::PivotNode::create();
						draw_node->addChild( pivot_node, std::numeric_limits<int>::max() );
					}

					// Action
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
					const Size layout_size( 80.f, 80.f );

					auto layout = ui::Layout::create();
					layout->setContentSize( layout_size );
					layout->setBackGroundColorType( ui::Layout::BackGroundColorType::SOLID );
					layout->setBackGroundColor( Color3B::GRAY );
					layout->setClippingEnabled( false );
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
						const float required_height = std::sqrt( pow( layout_size.width * 0.5f, 2 ) + pow( layout_size.height * 0.5f, 2 ) );

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
				}

				//
				// Research x 3
				//
				{
					const Size layout_size( 100.f, 100.f );

					auto layout = ui::Layout::create();
					layout->setContentSize( layout_size );
					layout->setBackGroundColorType( ui::Layout::BackGroundColorType::SOLID );
					layout->setBackGroundColor( Color3B::RED );
					layout->setClippingEnabled( true );
					layout->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.85f, visibleSize.height * 0.5f )
						- Vec2( layout->getContentSize().width * 0.75f, layout->getContentSize().height * 0.5f )
					);
					addChild( layout );

					// Pivot
					{
						auto pivot_node = cpg_node::PivotNode::create();
						layout->addChild( pivot_node, std::numeric_limits<int>::max() );
					}

					// Polygon
					{
						const float required_height = std::sqrt( pow( layout_size.width * 0.5f, 2 ) + pow( layout_size.height * 0.5f, 2 ) );

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
						auto layer = LayerColor::create( Color4B::BLACK, layout_size.width -2.f, layout_size.height - 2.f );
						layer->setVisible( true );
						layer->setPosition( 1.f, 1.f );
						layout->addChild( layer, 1 );
					}
				}

				return true;
			}

			void ChampionSelectButtonStepScene::onEnter()
			{
				Scene::onEnter();

				assert( !mKeyboardListener );
				mKeyboardListener = EventListenerKeyboard::create();
				mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ChampionSelectButtonStepScene::onKeyPressed, this );
				getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
			}
			void ChampionSelectButtonStepScene::onExit()
			{
				assert( mKeyboardListener );
				getEventDispatcher()->removeEventListener( mKeyboardListener );
				mKeyboardListener = nullptr;

				Scene::onExit();
			}


			void ChampionSelectButtonStepScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
			{
				if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
				{
					helper::BackToThePreviousScene::MoveBack();
					return;
				}
			}
		}
	}
}
