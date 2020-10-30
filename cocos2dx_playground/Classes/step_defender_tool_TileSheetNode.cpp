#include "step_defender_tool_TileSheetNode.h"

#include <new>
#include <numeric>

#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

USING_NS_CC;

namespace step_defender
{
	namespace tool
	{
		TileSheetNode::TileSheetNode( const Config& config ) : mConfig( config ), mIndicator( nullptr )
		{}

		TileSheetNode* TileSheetNode::create( const Config& config )
		{
			CCASSERT( 0 < config.TileWidth && 0 < config.TileHeight, "Failed - TileSheetNode::create" );

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

			auto texture = _director->getTextureCache()->getTextureForKey( mConfig.TexturePath );
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
			}

			//
			// Mouse Interface
			//
			{
				auto button = ui::Button::create( "guide_01_3.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setAnchorPoint( Vec2::ZERO );
				button->setScale9Enabled( true );
				button->setContentSize( getContentSize() );
				button->addTouchEventListener( CC_CALLBACK_2( TileSheetNode::onButton, this ) );
				addChild( button, std::numeric_limits<int>::min() );
			}

			//
			// Indicator
			//
			{
				auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "white_2x2.png" );
				sprite->setScale9Enabled( true );
				sprite->setContentSize( CC_SIZE_PIXELS_TO_POINTS( Size( mConfig.TileWidth, mConfig.TileHeight ) ) );
				sprite->setVisible( false );
				addChild( sprite, std::numeric_limits<int>::max() );

				mIndicator = sprite;
			}

			return true;
		}

		void TileSheetNode::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			auto button = static_cast<ui::Button*>( sender );

			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				const auto touch_position = convertToNodeSpace( button->getTouchBeganPosition() );
				mIndicator->setPosition( touch_position );
				mIndicator->setVisible( true );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				const auto touch_position = convertToNodeSpace( button->getTouchMovePosition() );
				mIndicator->setPosition( touch_position );
			}
			else if( ui::Widget::TouchEventType::ENDED == touch_event_type )
			{
				mIndicator->setVisible( false );
			}
			else if( ui::Widget::TouchEventType::CANCELED == touch_event_type )
			{
				mIndicator->setVisible( false );
			}
		}
	}
}
