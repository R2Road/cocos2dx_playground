#include "step_pathfinder_tool_ui_TileSelectNode.h"

#include <new>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const int TAG_Indicator = 20140416;
}

namespace step_pathfinder
{
	namespace tool_ui
	{
		TileSelectNode::TileSelectNode( const TileSelectCallback& tile_select_callback ) :
			mTileSelectCallback( tile_select_callback )
		{}

		TileSelectNode* TileSelectNode::create( const TileSelectCallback& tile_select_callback )
		{
			auto ret = new ( std::nothrow ) TileSelectNode( tile_select_callback );
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

		bool TileSelectNode::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			const auto menu_size = step_pathfinder::game::GetMaxMenuSize();

			//
			// Menu
			//
			int menu_index = 0;
			{
				const auto select_callback = CC_CALLBACK_2( TileSelectNode::onSelect, this );

				for( int cur = static_cast<int>( step_pathfinder::game::eTileType::FIRST ), end = static_cast<int>( step_pathfinder::game::eTileType::SIZE ); cur < end; ++cur )
				{
					const auto& tile_data = step_pathfinder::game::TileType2TileData( static_cast<step_pathfinder::game::eTileType>( cur ) );
					if( !tile_data.bToolEnable )
					{
						continue;
					}

					auto button = makeMenuButton(
						menu_size
						, tile_data.TileType
						, tile_data.Name
						, select_callback
					);
					button->setPositionY( menu_size.height * menu_index );
					addChild( button );

					++menu_index;
				}
			}

			//
			// Indicator
			//
			{
				auto indicator = ui::Scale9Sprite::createWithSpriteFrameName( "s9_guide_01_0.png" );
				indicator->setAnchorPoint( Vec2::ZERO );
				indicator->setTag( TAG_Indicator );
				indicator->setContentSize( menu_size );
				indicator->setColor( Color3B::GREEN );
				addChild( indicator, 1 );
			}

			//
			// Content Size
			//
			setContentSize( Size(
				menu_size.width
				, menu_size.height * menu_index
			) );

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 58, 0, 61, 255 ), getContentSize().width, getContentSize().height );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			return true;
		}


		Node* TileSelectNode::makeMenuButton( const cocos2d::Size menu_size, const step_pathfinder::game::eTileType tile_type, const char* button_text, const ui::Widget::ccWidgetTouchCallback& callback ) const
		{
			auto button = ui::Button::create( "guide_01_2.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
			button->setTag( static_cast<int>( tile_type ) );
			button->setAnchorPoint( Vec2::ZERO );
			button->addTouchEventListener( callback );
			button->setScale9Enabled( true );
			button->setContentSize( menu_size );
			{
				auto label = Label::createWithTTF( button_text, cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				button->setTitleLabel( label );
			}

			return button;
		}


		void TileSelectNode::SetIndicator( const step_pathfinder::game::eTileType tile_type )
		{
			auto button_node = getChildByTag( static_cast<int>( tile_type ) );

			// setup indicator
			auto indicator_node = getChildByTag( TAG_Indicator );
			indicator_node->setPosition( button_node->getPosition() );
		}

		void TileSelectNode::onSelect( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			auto sender_node = static_cast<Node*>( sender );

			// setup indicator
			auto indicator_node = getChildByTag( TAG_Indicator );
			indicator_node->setPosition( sender_node->getPosition() );

			mTileSelectCallback( static_cast<step_pathfinder::game::eTileType>( sender_node->getTag() ) );
		}
	}
}
