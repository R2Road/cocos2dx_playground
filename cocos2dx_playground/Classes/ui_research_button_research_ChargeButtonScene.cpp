#include "ui_research_button_research_ChargeButtonScene.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_Clamp.h"
#include "cpg_node_PivotNode.h"
#include "cpg_ui_ChargeButtonNode.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	class PushedViewNode : public cpg_ui::ChargeButtonPushedNode
	{
	private:
		PushedViewNode() : mViewNode( nullptr ) {}

	public:
		static PushedViewNode* create( const cocos2d::Size& size )
		{
			auto ret = new ( std::nothrow ) PushedViewNode();
			if( !ret || !ret->init( size ) )
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

		bool init( const cocos2d::Size& button_size )
		{
			if( !cpg_ui::ChargeButtonPushedNode::init() )
			{
				return false;
			}

			setContentSize( button_size );

			// Pivot
			addChild( cpg_node::PivotNode::create(), std::numeric_limits<int>::min() );

			// View
			mViewNode = LayerColor::create( Color4B::RED, button_size.width, button_size.height );
			mViewNode->setContentSize( button_size );
			addChild( mViewNode );

			// Setup
			SetChargeRate( 0.f );

			return true;
		}

		void SetChargeRate( const float charge_rate ) override
		{
			mViewNode->setOpacity( static_cast<unsigned int>(
				255.f * cpg::clamp( charge_rate, 0.f, 1.f )
			) ) ;

			mViewNode->setContentSize( Size( getContentSize().width, getContentSize().height * charge_rate ) );
		}

	private:
		Node* mViewNode;
	};
}

namespace ui_research
{
	namespace button_research
	{
		ChargeButtonScene::ChargeButtonScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* ChargeButtonScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) ChargeButtonScene( back_to_the_previous_scene_callback );
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

		bool ChargeButtonScene::init()
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
			// Button Status View
			//
			auto button_status_label = Label::createWithTTF( "-", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			button_status_label->setColor( Color3B::GREEN );
			button_status_label->setPosition(
				Vec2( visibleCenter.x, 0.f )
				+ Vec2( 0.f, visibleSize.height * 0.7f )
			);
			addChild( button_status_label, std::numeric_limits<int>::max() );

			//
			// Research
			//
			{
				const Size button_size( 100.f, 100.f );

				auto ex_button = cpg_ui::ChargeButtonNode::create( button_size, 2.f );
				ex_button->setPosition( visibleCenter );
				addChild( ex_button );
				
				// Background
				{
					ex_button->SetBackground( LayerColor::create( Color4B::BLACK, 100u, 100u ) );
				}

				// Normal View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_0.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( button_size );

					ex_button->SetView( cpg_ui::ChargeButtonNode::eViewIndex::Normal, sprite );
				}

				// Mouse Over View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_1.png" );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( button_size );
					sprite->setVisible( false );

					ex_button->SetView( cpg_ui::ChargeButtonNode::eViewIndex::MouseOver, sprite );
				}

				// Push View
				{
					auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_2.png" );
					sprite->setVisible( false );
					sprite->setAnchorPoint( Vec2::ZERO );
					sprite->setContentSize( button_size );

					ex_button->SetView( cpg_ui::ChargeButtonNode::eViewIndex::Push, sprite );
				}

				// Pushed View
				{
					auto view_node = PushedViewNode::create( button_size - Size( 10.f, 10.f ) );
					view_node->setVisible( false );
					view_node->setPosition( 5.f, 5.f );

					ex_button->SetPushedView( view_node );
				}

				ex_button->SetCallback( [button_status_label]( const cpg_ui::ChargeButtonNode::eButtonEvent button_event )
				{
					switch( button_event )
					{
					case cpg_ui::ChargeButtonNode::eButtonEvent::MouseOver:
						CCLOG( "MouseOver" );
						break;
					case cpg_ui::ChargeButtonNode::eButtonEvent::MouseLeave:
						CCLOG( "MouseLeave" );
						break;
					case cpg_ui::ChargeButtonNode::eButtonEvent::Push:
						CCLOG( "Push" );
						button_status_label->setString( "Charging" );
						break;
					case cpg_ui::ChargeButtonNode::eButtonEvent::Move:
						CCLOG( "Move" );
						break;
					case cpg_ui::ChargeButtonNode::eButtonEvent::Release:
						CCLOG( "Release" );
						button_status_label->setString( "-" );
						break;
					case cpg_ui::ChargeButtonNode::eButtonEvent::Charged:
						CCLOG( "Charged" );
						button_status_label->setString( "Charged" );
						break;
					}
				} );
			}

			return true;
		}

		void ChargeButtonScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ChargeButtonScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ChargeButtonScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void ChargeButtonScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
