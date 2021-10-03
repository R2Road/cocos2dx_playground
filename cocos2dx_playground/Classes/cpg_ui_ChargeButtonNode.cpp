#include "cpg_ui_ChargeButtonNode.h"

#include <cstdlib>
#include <new>
#include <numeric>

#include "2d/CCCamera.h"
#include "2d/CCLayer.h"
#include "ui/UILayout.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerMouse.h"

#include "cpg_node_PivotNode.h"

USING_NS_CC;

namespace cpg_ui
{
	ChargeButtonNode::ChargeButtonNode() :
		mMouseEventListener( nullptr )
		, mbOnMouseOver( false )

		, mViewNodes()
		, mPushedNode( nullptr )
		, mBackgroundNode( nullptr )

		, mOnButtonCallback( nullptr )

		, mCharge( 0.f )
	{
		memset( mViewNodes, 0, eViewIndex::SIZE * sizeof( Node* ) );
	}

	ChargeButtonNode* ChargeButtonNode::create( const Size& size )
	{
		auto ret = new ( std::nothrow ) ChargeButtonNode();
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

	bool ChargeButtonNode::init( const Size& size )
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
			addTouchEventListener( [this]( Ref* /*sender*/, ui::Widget::TouchEventType touch_event_type )
			{
				if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
				{
					onButton( eButtonEvent::Push );
				}
				else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
				{
					onButton( eButtonEvent::Move );
				}
				else //if( ui::Widget::TouchEventType::ENDED == touch_event_type || ui::Widget::TouchEventType::CANCELED == touch_event_type )
				{
					onButton( eButtonEvent::Release );
				}
			} );
		}

		return true;
	}

	void ChargeButtonNode::onEnter()
	{
		ui::Widget::onEnter();

		CCASSERT( !mMouseEventListener, "" );
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
	void ChargeButtonNode::onExit()
	{
		mbOnMouseOver = false;

		CCASSERT( mMouseEventListener, "" );
		getEventDispatcher()->removeEventListener( mMouseEventListener );
		mMouseEventListener = nullptr;

		ui::Widget::onExit();
	}

	void ChargeButtonNode::SetPushedView( ChargeButtonPushedNode* pushed_node )
	{
		if( mPushedNode )
		{
			removeChild( mPushedNode );
		}

		mPushedNode = pushed_node;
		if( mPushedNode )
		{
			addChild( mPushedNode, -1 );
		}
	}
	void ChargeButtonNode::SetView( const eViewIndex view_index, Node* node )
	{
		if( mViewNodes[view_index] )
		{
			removeChild( mViewNodes[view_index] );
		}

		mViewNodes[view_index] = node;
		if( node )
		{
			addChild( node, view_index );
		}
	}
	void ChargeButtonNode::SetBackground( Node* node )
	{
		if( mBackgroundNode )
		{
			removeChild( mBackgroundNode );
		}

		mBackgroundNode = node;
		if( node )
		{
			addChild( node, std::numeric_limits<int>::min() );
		}
	}

	void ChargeButtonNode::onButton( const eButtonEvent button_event )
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
			setPushedViewVisible( true );
			mCharge = 0.f;
			updatePushedView( mCharge );
			schedule( schedule_selector( ChargeButtonNode::update4Charge ) );
			break;
		case eButtonEvent::Move:
			break;
		case eButtonEvent::Release:
			setPushedViewVisible( false );
			unschedule( schedule_selector( ChargeButtonNode::update4Charge ) );
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

		if( mOnButtonCallback )
		{
			mOnButtonCallback( button_event );
		}
	}

	void ChargeButtonNode::showView( const eViewIndex view_index )
	{
		for( auto v : mViewNodes )
		{
			if( v )
			{
				v->setVisible( false );
			}
		}

		if( mViewNodes[view_index] )
		{
			mViewNodes[view_index]->setVisible( true );
		}
	}

	void ChargeButtonNode::update4Charge( const float dt )
	{
		mCharge = std::min( 1.f, mCharge += dt );
		updatePushedView( mCharge );

		if( 0.000001 > std::abs( 1.f - mCharge ) )
		{
			mOnButtonCallback( eButtonEvent::Charged );
			unschedule( schedule_selector( ChargeButtonNode::update4Charge ) );
		}
	}
	void ChargeButtonNode::setPushedViewVisible( const bool visible )
	{
		if( mPushedNode )
		{
			mPushedNode->setVisible( visible );
		}
	}
	void ChargeButtonNode::updatePushedView( const float charge_rate )
	{
		if( mPushedNode )
		{
			mPushedNode->SetChargeRate( charge_rate );
		}
	}
}