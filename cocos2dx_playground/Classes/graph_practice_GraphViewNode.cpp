#include "graph_practice_GraphViewNode.h"

#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "ui/UIScale9Sprite.h"

USING_NS_CC;

namespace graph_practice
{
	GraphViewNode::GraphViewNode() : mViewNode( nullptr ) {}

	GraphViewNode* GraphViewNode::create( const int width, const int height )
	{
		auto ret = new ( std::nothrow ) GraphViewNode();
		if( !ret || !ret->init( width, height ) )
		{
			delete ret;
			ret = nullptr;
			return nullptr;
		}
		else
		{
			ret->autorelease();
		}

		return ret;
	}

	bool GraphViewNode::init( const int width, const int height )
	{
		if( !Node::init() )
		{
			return false;
		}

		setContentSize( Size( width, height ) );

		//
		// Graph View
		//
		{
			auto view_node = Node::create();
			addChild( view_node );

			// Pivot
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 4.f );
				view_node->addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 100, 100, 100, 150 ), getContentSize().width, getContentSize().height );
				view_node->addChild( background_layer, -2 );
			}

			//
			// x, y
			//
			{
				auto x_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
				x_view->setScale9Enabled( true );
				x_view->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
				x_view->setContentSize( Size( getContentSize().width, 2.f ) );
				x_view->setColor( Color3B::BLUE );
				view_node->addChild( x_view );

				auto y_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
				y_view->setScale9Enabled( true );
				y_view->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT );
				y_view->setContentSize( Size( 2.f, getContentSize().height ) );
				y_view->setColor( Color3B::BLUE );
				view_node->addChild( y_view );
			}
		}

		//
		// Current Value Indicator
		//
		{
			mViewNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
			mViewNode->setScale( 4.f );
			addChild( mViewNode );
		}

		return true;
	}

	void GraphViewNode::UpdateView( const float g_x, const float g_y )
	{
		mViewNode->setPosition(
			mViewNode->getParent()->getContentSize().width * g_x
			, mViewNode->getParent()->getContentSize().height * g_y
		);
	}
}
