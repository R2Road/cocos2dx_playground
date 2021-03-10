#include "ui_research_button_research_team_fight_manager_ChampionSelectButtonTestScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCDrawNode.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "ui/UILayout.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_animation_InfoContainer.h"
#include "cpg_node_PivotNode.h"
#include "cpg_ui_EXButtonNode.h"
#include "cpg_StringTable.h"

#include "step_mole_AnimationComponent.h"

#include "ui_research_button_research_team_fight_manager_OnMouseOverNode.h"

USING_NS_CC;

namespace
{
	const cpg::animation::InfoContainerT& GetCharacterAnimationInfoContainer()
	{
		static const cpg::animation::InfoContainerT animation_info_container = {
			{
				cpg::animation::eIndex::sleep
				, 0.1f
				, std::vector<std::string>{ "dummy_actor_001_slp_1_0.png" }
			}
			,{
				cpg::animation::eIndex::idle
				, 0.075f
				, std::vector<std::string>{ "dummy_actor_001_idl_1_0.png", "dummy_actor_001_idl_1_1.png", "dummy_actor_001_idl_1_2.png", "dummy_actor_001_idl_1_3.png", "dummy_actor_001_idl_1_4.png", "dummy_actor_001_idl_1_4.png", "dummy_actor_001_idl_1_4.png", "dummy_actor_001_idl_1_4.png" }
			}
			,{
				cpg::animation::eIndex::run
				, 0.075f
				, std::vector<std::string>{ "dummy_actor_001_mov_1_0.png", "dummy_actor_001_mov_1_1.png", "dummy_actor_001_mov_1_2.png", "dummy_actor_001_mov_1_3.png" }
			}
		};

		return animation_info_container;
	}
}

namespace ui_research
{
	namespace button_research
	{
		namespace team_fight_manager
		{
			ChampionSelectButtonTestScene::ChampionSelectButtonTestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
				helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
				, mKeyboardListener( nullptr )
			{}

			Scene* ChampionSelectButtonTestScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
			{
				auto ret = new ( std::nothrow ) ChampionSelectButtonTestScene( back_to_the_previous_scene_callback );
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

			bool ChampionSelectButtonTestScene::init()
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
				// Research
				//
				{
					const Size button_size( 100.f, 100.f );

					auto ex_button = cpg_ui::EXButtonNode::create( button_size );
					ex_button->setPosition( visibleCenter );
					addChild( ex_button );

					ex_button->SetBackground( LayerColor::create( Color4B::BLACK, 100u, 100u ) );

					// Normal
					{
						auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_0.png" );
						sprite->setAnchorPoint( Vec2::ZERO );
						sprite->setContentSize( button_size );

						ex_button->SetView( cpg_ui::EXButtonNode::eViewIndex::Normal, sprite );
					}

					// Mouse Over
					{
						auto on_mouse_over_node = team_fight_manager::OnMouseOverNode::create( button_size );
						on_mouse_over_node->setVisible( false );

						ex_button->SetView( cpg_ui::EXButtonNode::eViewIndex::MouseOver, on_mouse_over_node );
					}

					// Push
					{
						auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_2.png" );
						sprite->setVisible( false );
						sprite->setAnchorPoint( Vec2::ZERO );
						sprite->setContentSize( button_size );

						ex_button->SetView( cpg_ui::EXButtonNode::eViewIndex::Push, sprite );
					}

					// Character
					step_mole::AnimationComponent* animation_component = nullptr;
					{
						auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_2.png" );
						sprite->setScale( 6.f );
						sprite->setPosition( ex_button->getContentSize().width * 0.5f, ex_button->getContentSize().height * 0.5f );
						ex_button->addChild( sprite, 1 );

						// Animation Component
						animation_component = step_mole::AnimationComponent::create( GetCharacterAnimationInfoContainer() );
						sprite->addComponent( animation_component );
						animation_component->PlayAnimation( cpg::animation::eIndex::sleep );
					}

					ex_button->SetCallback( [animation_component]( const cpg_ui::EXButtonNode::eButtonEvent button_event )
					{
						switch( button_event )
						{
						case cpg_ui::EXButtonNode::eButtonEvent::MouseOver:
							CCLOG( "MouseOver" );
							animation_component->PlayAnimation( cpg::animation::eIndex::idle );
							break;
						case cpg_ui::EXButtonNode::eButtonEvent::MouseLeave:
							CCLOG( "MouseLeave" );
							animation_component->PlayAnimation( cpg::animation::eIndex::sleep );
							break;
						case cpg_ui::EXButtonNode::eButtonEvent::Push:
							CCLOG( "Push" );
							animation_component->PlayAnimation( cpg::animation::eIndex::run );
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

			void ChampionSelectButtonTestScene::onEnter()
			{
				Scene::onEnter();

				assert( !mKeyboardListener );
				mKeyboardListener = EventListenerKeyboard::create();
				mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ChampionSelectButtonTestScene::onKeyPressed, this );
				getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
			}
			void ChampionSelectButtonTestScene::onExit()
			{
				assert( mKeyboardListener );
				getEventDispatcher()->removeEventListener( mKeyboardListener );
				mKeyboardListener = nullptr;

				Scene::onExit();
			}


			void ChampionSelectButtonTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
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
