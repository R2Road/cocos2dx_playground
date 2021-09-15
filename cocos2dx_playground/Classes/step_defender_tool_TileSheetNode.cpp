#include "step_defender_tool_TileSheetNode.h"

#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_Clamp.h"

USING_NS_CC;

namespace step_defender
{
	namespace tool
	{
		TileSheetNode::TileSheetNode( const cpg::TileSheetConfiguration& config ) :
			mConfig( config )
			, mPosition2GridIndexConverter( mConfig.GetBlockWidth(), mConfig.GetBlockHeight() )
			, mSelectCallback( nullptr )

			, mIndicator( nullptr )
			, mLastSelectedPoint()
		{}

		TileSheetNode* TileSheetNode::create( const cpg::TileSheetConfiguration& config )
		{
			CCASSERT( 0 < config.GetTileWidth() && 0 < config.GetTileHeight(), "Failed - TileSheetNode::create" );

			auto ret = new ( std::nothrow ) TileSheetNode( config );
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

		bool TileSheetNode::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			auto texture = _director->getTextureCache()->addImage( mConfig.GetTexturePath() );
			CCASSERT( nullptr != texture, "Texture Nothing" );

			texture->setAliasTexParameters();
			setContentSize( texture->getContentSize() );

			//
			// Pivot
			//
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 2.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Sheet View
			//
			{
				auto sprite = Sprite::createWithTexture( texture );
				sprite->setAnchorPoint( Vec2::ZERO );
				addChild( sprite );

				// Guide
				auto guide = LayerColor::create( Color4B( 0u, 0u, 0u, 60u ), sprite->getContentSize().width, sprite->getContentSize().height );
				addChild( guide, -1 );
			}

			//
			// Mouse Interface
			//
			{
				const Size Margin( 10.f, 10.f );

				auto button = ui::Button::create( "guide_01_3.png", "guide_01_3.png", "guide_01_3.png", ui::Widget::TextureResType::PLIST );
				button->setAnchorPoint( Vec2::ZERO );
				button->setScale9Enabled( true );
				button->setContentSize( Margin + getContentSize() + Margin );
				button->addTouchEventListener( CC_CALLBACK_2( TileSheetNode::onButton, this ) );
				button->setPosition( Vec2( -Margin.width, -Margin.height ) );
				addChild( button, std::numeric_limits<int>::min() );
			}

			//
			// Indicator
			//
			{
				auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "s9_guide_01_0.png" );
				sprite->setAnchorPoint( Vec2::ZERO );
				sprite->setScale9Enabled( true );
				sprite->setContentSize( CC_SIZE_PIXELS_TO_POINTS( Size( mConfig.GetBlockWidth(), mConfig.GetBlockHeight() ) ) );
				sprite->setColor( Color3B::GREEN );
				addChild( sprite, std::numeric_limits<int>::max() );

				mIndicator = sprite;
			}

			//
			// Setup
			//
			updateSelectedPoint( Vec2::ZERO );
			updateIndicatorPosition();

			return true;
		}

		void TileSheetNode::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			auto button = static_cast<ui::Button*>( sender );

			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				updateSelectedPoint( button->getTouchBeganPosition() );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				updateSelectedPoint( button->getTouchMovePosition() );
			}
			else if( ui::Widget::TouchEventType::ENDED == touch_event_type || ui::Widget::TouchEventType::CANCELED == touch_event_type )
			{
				updateSelectedPoint( button->getTouchEndPosition() );
			}

			if( mSelectCallback )
			{
				mSelectCallback( mLastSelectedPoint.x, mLastSelectedPoint.y );
			}
			updateIndicatorPosition();
		}
		void TileSheetNode::updateSelectedPoint( const cocos2d::Vec2& world_position )
		{
			const auto node_space_position = convertToNodeSpace( world_position );

			const Vec2 fixed_position( cpg::clamp( node_space_position.x, 0.f, getContentSize().width - 1.f ), cpg::clamp( node_space_position.y, 0.f, getContentSize().height - 1.f ) );
			const auto scaled_position = fixed_position * _director->getContentScaleFactor();

			const auto touch_point = mPosition2GridIndexConverter.Position2Point( scaled_position.x, scaled_position.y );
			mLastSelectedPoint.x = touch_point.x;
			mLastSelectedPoint.y = touch_point.y;
		}
		void TileSheetNode::updateIndicatorPosition()
		{
			mIndicator->setPosition(
				mLastSelectedPoint.x * mIndicator->getContentSize().width
				, mLastSelectedPoint.y * mIndicator->getContentSize().height
			);
		}
	}
}
