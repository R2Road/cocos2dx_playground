#include "graph_practice_GraphViewNode.h"

#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCLabel.h"
#include "ui/UIScale9Sprite.h"

USING_NS_CC;

namespace graph_practice
{
	GraphViewNode::GraphViewNode( const EvaluatorFunc& evaluator_func ) : mViewNode( nullptr ), mEvaluatorFunc( evaluator_func ) {}

	GraphViewNode* GraphViewNode::create( const char* title, const int width, const int height, const EvaluatorFunc& evaluator_func )
	{
		auto ret = new ( std::nothrow ) GraphViewNode( evaluator_func );
		if( !ret || !ret->init( title, width, height ) )
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

	bool GraphViewNode::init( const char* title, const int width, const int height )
	{
		if( !Node::init() )
		{
			return false;
		}

		setContentSize( Size( width, height ) );

		//
		// Title
		//
		{
			auto label = Label::createWithTTF( title, "fonts/arial.ttf", 9 );
			label->setPosition( Vec2(
				getContentSize().width * 0.5f
				, -label->getContentSize().height
			) );
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Graph View
		//
		{
			auto view_node = Node::create();
			addChild( view_node );

			// Pivot
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 2.f );
				view_node->addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			{
				auto background = LayerColor::create( Color4B( 100, 100, 100, 150 ), getContentSize().width, getContentSize().height );
				view_node->addChild( background, -1 );
			}

			//
			// Guide
			//
			{
				const float Spacing = width * 0.1f;
				for( int i = 1; 10 > i; ++i )
				{
					auto guide_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
					guide_view->setScale9Enabled( true );
					guide_view->setAnchorPoint( Vec2( 0.5f, 0.f ) );
					guide_view->setContentSize( Size( 2.f, getContentSize().height ) );
					guide_view->setColor( Color3B( 100u, 100u, 100u ) );
					guide_view->setPositionX( Spacing * i );
					view_node->addChild( guide_view, -2 );
				}

				for( int i = 1; 10 > i; ++i )
				{
					auto guide_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
					guide_view->setScale9Enabled( true );
					guide_view->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
					guide_view->setContentSize( Size( getContentSize().height, 2.f ) );
					guide_view->setColor( Color3B( 100u, 100u, 100u ) );
					guide_view->setPositionY( Spacing * i );
					view_node->addChild( guide_view, -2 );
				}
			}

			//
			// x, y
			//
			{
				auto x_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
				x_view->setScale9Enabled( true );
				x_view->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
				x_view->setContentSize( Size( getContentSize().width, 2.f ) );
				x_view->setColor( Color3B::GRAY );
				view_node->addChild( x_view );

				auto y_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
				y_view->setScale9Enabled( true );
				y_view->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT );
				y_view->setContentSize( Size( 2.f, getContentSize().height ) );
				y_view->setColor( Color3B::GRAY );
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

	void GraphViewNode::UpdateView( const float g_x )
	{
		const auto g_y = mEvaluatorFunc( g_x );

		mViewNode->setPosition(
			mViewNode->getParent()->getContentSize().width * g_x
			, mViewNode->getParent()->getContentSize().height * g_y
		);
	}
}
