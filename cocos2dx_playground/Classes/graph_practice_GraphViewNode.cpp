#include "graph_practice_GraphViewNode.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCLabel.h"
#include "2d/CCSpriteBatchNode.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_StringTable.h"

USING_NS_CC;

namespace graph_practice
{
	GraphViewNode::GraphViewNode( const int part_width, const int part_height, const EvaluatorFunc& evaluator_func ) :
		mPartSize( std::max( 50, part_width ), std::max( 50, part_height ) )
		, mEvaluatorFunc( evaluator_func )

		, mIndicatorNode( nullptr )
		, mIndicatorXNode( nullptr )
		, mIndicatorYNode( nullptr )
		, mIndicatorBridgeXNode( nullptr )
		, mIndicatorBridgeYNode( nullptr )
		, mIndicatorY2Node( nullptr )
	{}

	GraphViewNode* GraphViewNode::create( const char* title, const int part_width, const int part_height, const EvaluatorFunc& evaluator_func )
	{
		auto ret = new ( std::nothrow ) GraphViewNode( part_width, part_height, evaluator_func );
		if( !ret || !ret->init( title ) )
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

	bool GraphViewNode::init( const char* title )
	{
		if( !Node::init() )
		{
			return false;
		}

		const int HeaderHeight = 10;
		setContentSize( mPartSize + Size( 0.f, HeaderHeight ) );

		// Pivot
		{
			auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
			pivot->setScale( 2.f );
			addChild( pivot, std::numeric_limits<int>::max() );
		}

		//
		// Background Guide
		//
		{
			auto layer = LayerColor::create( Color4B::BLUE, getContentSize().width, getContentSize().height );
			layer->setAnchorPoint( Vec2::ZERO );
			addChild( layer, std::numeric_limits<int>::min() );
		}

		//
		// Graph View
		//
		{
			auto view_node = Node::create();
			view_node->setContentSize( mPartSize );
			addChild( view_node );

			//
			// Background
			//
			{
				auto background = LayerColor::create( Color4B( 44u, 44u, 44u, 255u ), view_node->getContentSize().width, view_node->getContentSize().height );
				view_node->addChild( background, -2 );
			}

			//
			// Guide
			//
			{
				const Color3B GuideColor( 55u, 55u, 55u );

				// Vertical
				{
					const float Spacing = mPartSize.width * 0.1f;
					for( int i = 1; 10 > i; ++i )
					{
						auto guide_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
						guide_view->setScale9Enabled( true );
						guide_view->setAnchorPoint( Vec2( 0.5f, 0.f ) );
						guide_view->setContentSize( Size( 1.f, view_node->getContentSize().height ) );
						guide_view->setColor( GuideColor );
						guide_view->setPositionX( Spacing * i );
						view_node->addChild( guide_view, -1 );
					}
				}

				// Horizontal
				{
					const float Spacing = mPartSize.height * 0.1f;
					for( int i = 1; 10 > i; ++i )
					{
						auto guide_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
						guide_view->setScale9Enabled( true );
						guide_view->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
						guide_view->setContentSize( Size( view_node->getContentSize().height, 1.f ) );
						guide_view->setColor( GuideColor );
						guide_view->setPositionY( Spacing * i );
						view_node->addChild( guide_view, -1 );
					}
				}
			}

			//
			// x, y
			//
			{
				auto x_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
				x_view->setScale9Enabled( true );
				x_view->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
				x_view->setContentSize( Size( view_node->getContentSize().width, 2.f ) );
				x_view->setColor( Color3B::GRAY );
				view_node->addChild( x_view );

				auto y_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
				y_view->setScale9Enabled( true );
				y_view->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT );
				y_view->setContentSize( Size( 2.f, view_node->getContentSize().height ) );
				y_view->setColor( Color3B::GRAY );
				view_node->addChild( y_view );

				auto diagonal_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
				diagonal_view->setScale9Enabled( true );
				diagonal_view->setAnchorPoint( Vec2( 0.5f, 0.0f ) );
				diagonal_view->setContentSize( Size( 2.f, sqrt( pow( view_node->getContentSize().width, 2 ) + pow( view_node->getContentSize().height, 2 ) ) ) );
				diagonal_view->setColor( Color3B::GRAY );
				diagonal_view->setRotation( 45.f );
				view_node->addChild( diagonal_view );
			}

			//
			//
			//
			{
				auto batch_node = SpriteBatchNode::createWithTexture( _director->getTextureCache()->getTextureForKey( "textures/texture_001.png" ), 100u );
				view_node->addChild( batch_node, 1 );
				{
					auto shadow_view = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
					shadow_view->setColor( Color3B::GREEN );
					float g_x = 0.f;
					float g_y = 0.f;
					for( int i = 0; 100 > i; ++i )
					{
						g_x = 0.01f * i;
						g_y = mEvaluatorFunc( g_x );

						shadow_view->setPosition(
							view_node->getContentSize().width * g_x
							, view_node->getContentSize().height * g_y
						);

						batch_node->insertQuadFromSprite( shadow_view, i );
					}
				}
			}

			//
			// Current Value Indicator
			//
			{
				mIndicatorNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorNode->setScale( 6.f );
				mIndicatorNode->setColor( Color3B( 73, 179, 255 ) );
				view_node->addChild( mIndicatorNode, 4 );

				mIndicatorXNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorXNode->setScale( 6.f );
				mIndicatorXNode->setColor( Color3B::RED );
				view_node->addChild( mIndicatorXNode, 3 );

				mIndicatorYNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorYNode->setScale( 6.f );
				mIndicatorYNode->setColor( Color3B::RED );
				view_node->addChild( mIndicatorYNode, 3 );

				mIndicatorBridgeXNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorBridgeXNode->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				mIndicatorBridgeXNode->setOpacity( 80u );
				view_node->addChild( mIndicatorBridgeXNode, 2 );

				mIndicatorBridgeYNode = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorBridgeYNode->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
				mIndicatorBridgeYNode->setOpacity( 80u );
				view_node->addChild( mIndicatorBridgeYNode, 2 );

				mIndicatorY2Node = Sprite::createWithSpriteFrameName( "white_2x2.png" );
				mIndicatorY2Node->setScale( 6.f );
				mIndicatorY2Node->setColor( Color3B( 73, 179, 255 ) );
				mIndicatorY2Node->setPositionY( view_node->getContentSize().height + ( HeaderHeight * 0.5f ) );
				view_node->addChild( mIndicatorY2Node, 2 );
			}
		}

		return true;
	}

	void GraphViewNode::UpdateView( const float g_x )
	{
		const auto g_y = mEvaluatorFunc( g_x );

		mIndicatorNode->setPosition(
			mIndicatorNode->getParent()->getContentSize().width * g_x
			, mIndicatorNode->getParent()->getContentSize().height * g_y
		);

		mIndicatorXNode->setPositionX( mIndicatorNode->getParent()->getContentSize().width * g_x );
		mIndicatorYNode->setPositionY( mIndicatorNode->getParent()->getContentSize().height * g_y );

		mIndicatorBridgeXNode->setContentSize( Size( 1.f, std::abs( mIndicatorNode->getPositionY() ) ) );
		mIndicatorBridgeXNode->setScaleY( 0 <= mIndicatorNode->getPositionY() ? 1.f : -1.f );
		mIndicatorBridgeXNode->setPositionX( mIndicatorNode->getParent()->getContentSize().width * g_x );

		mIndicatorBridgeYNode->setContentSize( Size( mIndicatorNode->getPositionX(), 1.f ) );
		mIndicatorBridgeYNode->setScaleY( 0 <= mIndicatorNode->getPositionX() ? 1.f : -1.f );
		mIndicatorBridgeYNode->setPositionY( mIndicatorNode->getParent()->getContentSize().height * g_y );

		mIndicatorY2Node->setPositionX( mIndicatorNode->getParent()->getContentSize().width * g_y );
	}
}
