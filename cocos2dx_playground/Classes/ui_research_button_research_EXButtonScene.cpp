#include "ui_research_button_research_EXButtonScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCCamera.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventListenerMouse.h"

#include "cpg_StringTable.h"

#include "cpg_node_PivotNode.h"

USING_NS_CC;

namespace
{
	class EXButtonNode : public ui::Widget
	{
	public:
		enum class eButtonEvent
		{
			MouseOver,
			MouseLeave,
			Push,
			Move,
			Release
		};
		enum eViewIndex
		{
			Normal,
			MouseOver,
			Push,

			SIZE,
		};

	private:
		EXButtonNode() :
			mMouseEventListener( nullptr )
			, mbOnMouseOver( false )

			, mViewNodes()
		{
			memset( mViewNodes, 0, eViewIndex::SIZE * sizeof( Node* ) );
		}

	public:
		static EXButtonNode* EXButtonNode::create( const Size& size )
		{
			auto ret = new ( std::nothrow ) EXButtonNode();
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

	private:
		bool init( const Size& size )
		{
			if( !ui::Widget::init() )
			{
				return false;
			}

			setContentSize( size );
			setTouchEnabled( true );

			// pivot
			{
				addChild( cpg_node::PivotNode::create(), std::numeric_limits<int>::max() );
			}

			// area indicator
			{
				addChild( LayerColor::create(
					Color4B::BLUE, getContentSize().width, getContentSize().height )
					, std::numeric_limits<int>::min()
				);
			}

			//
			{
				addTouchEventListener( [this]( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type )
				{
					if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
					{
						onButton( eButtonEvent::Push );
						CCLOG( "push" );
					}
					else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
					{
						onButton( eButtonEvent::Move );
						CCLOG( "move" );
					}
					else //if( ui::Widget::TouchEventType::ENDED == touch_event_type || ui::Widget::TouchEventType::CANCELED == touch_event_type )
					{
						onButton( eButtonEvent::Release );
						CCLOG( "release" );
					}
				} );
			}

			return true;
		}

	public:
		void onEnter()
		{
			ui::Widget::onEnter();

			CCASSERT( !mMouseEventListener );
			mMouseEventListener = EventListenerMouse::create();
			mMouseEventListener->onMouseMove = [this]( EventMouse* event ) {

				//
				// 20210308
				// This code originated from "Widget::onTouchBegan"
				//

				const auto camera = Camera::getVisitingCamera();
				const auto current_hit_result = hitTest( Vec2( event->getCursorX(), event->getCursorY() ), camera, nullptr );

				if( !mbOnMouseOver && current_hit_result )
				{
					mbOnMouseOver = current_hit_result;

					event->stopPropagation();

					CCLOG( "on mouse over" );
					onButton( eButtonEvent::MouseOver );
				}
				else if( mbOnMouseOver && !current_hit_result )
				{
					mbOnMouseOver = current_hit_result;

					onButton( eButtonEvent::MouseLeave );
				}
			};
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mMouseEventListener, this );
		}
		void onExit()
		{
			mbOnMouseOver = false;

			CCASSERT( mMouseEventListener, "" );
			getEventDispatcher()->removeEventListener( mMouseEventListener );
			mMouseEventListener = nullptr;

			ui::Widget::onExit();
		}

		void SetView( const eViewIndex view_index, Node* node )
		{
			if( mViewNodes[view_index] )
			{
				removeChild( mViewNodes[view_index], view_index );
			}

			mViewNodes[view_index] = node;
			if( node )
			{
				addChild( node, view_index );
			}
		}

	private:
		void onButton( const eButtonEvent button_event )
		{
			switch( button_event )
			{
			case eButtonEvent::MouseOver:
				showView( eViewIndex::MouseOver );
				break;
			case eButtonEvent::MouseLeave:
				showView( eViewIndex::Normal );
				break;
			case eButtonEvent::Push:
				showView( eViewIndex::Push );
				break;
			case eButtonEvent::Move:
				break;
			case eButtonEvent::Release:
				if( mbOnMouseOver )
				{
					showView( eViewIndex::MouseOver );
				}
				else
				{
					showView( eViewIndex::Normal );
				}
				break;
			}
		}

		void showView( const eViewIndex view_index )
		{
			for( auto v : mViewNodes )
			{
				v->setVisible( false );
			}

			if( mViewNodes[view_index] )
			{
				mViewNodes[view_index]->setVisible( true );
			}
		}

	private:
		EventListenerMouse* mMouseEventListener;
		bool mbOnMouseOver;

		Node* mViewNodes[eViewIndex::SIZE];
	};
}

namespace ui_research
{
	namespace button_research
	{
		EXButtonScene::EXButtonScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* EXButtonScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) EXButtonScene( back_to_the_previous_scene_callback );
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

		bool EXButtonScene::init()
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
				auto ex_button = EXButtonNode::create( Size( 100.f, 100.f ) );
				ex_button->setPosition( visibleCenter );
				addChild( ex_button );
				
				ex_button->SetView( EXButtonNode::eViewIndex::Normal, Sprite::createWithSpriteFrameName( "guide_01_0.png" ) );
				ex_button->SetView( EXButtonNode::eViewIndex::MouseOver, Sprite::createWithSpriteFrameName( "guide_01_1.png" ) );
				ex_button->SetView( EXButtonNode::eViewIndex::Push, Sprite::createWithSpriteFrameName( "guide_01_2.png" ) );

				//ex_button->addTouchEventListener( CC_CALLBACK_2( EXButtonScene::onTouchWidget, this ) );
			}

			return true;
		}

		void EXButtonScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( EXButtonScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void EXButtonScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void EXButtonScene::onTouchWidget( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				CCLOG( "began" );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				CCLOG( "moved" );
			}
			else //if( ui::Widget::TouchEventType::ENDED == touch_event_type || ui::Widget::TouchEventType::CANCELED == touch_event_type )
			{
				CCLOG( "ended" );
			}
		}


		void EXButtonScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
