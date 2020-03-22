#include "step_pathfinder_tool_ui_TileSelectNode.h"

#include <new>

#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UITextField.h"

USING_NS_CC;

const int TAG_Indicator = 20140416;

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
				return nullptr;
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

			const auto menu_size = step_pathfinder::game::terrain::GetMaxMenuSize();
			const auto select_callback = CC_CALLBACK_2( TileSelectNode::onSelect, this );

			//
			// Menu
			//
			int menu_index = 0;
			{
				Vec2 pivot_position( menu_size.width * 0.5f, menu_size.height * 0.5f );
				for( int cur = static_cast<int>( step_pathfinder::game::terrain::eTileType::FIRST ), end = static_cast<int>( step_pathfinder::game::terrain::eTileType::SIZE ); cur < end; ++cur )
				{
					const auto& tile_data = step_pathfinder::game::terrain::TileType2TileData( static_cast<step_pathfinder::game::terrain::eTileType>( cur ) );
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
					button->setPosition(
						pivot_position
						+ Vec2( 0.f, button->getContentSize().height * menu_index )
					);
					addChild( button );

					++menu_index;
				}
			}

			//
			// Indicator
			//
			{
				auto indicator = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
				indicator->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				indicator->setTag( TAG_Indicator );
				indicator->setContentSize( menu_size );
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
				addChild( background_layer, -1 );
			}

			//
			// Start Setup
			//
			onSelect(
				getChildByTag( static_cast<int>( step_pathfinder::game::terrain::eTileType::road ) )
				, ui::Widget::TouchEventType::BEGAN
			);

			return true;
		}


		Node* TileSelectNode::makeMenuButton( const cocos2d::Size menu_size, const step_pathfinder::game::terrain::eTileType tile_type, const char* button_text, const ui::Widget::ccWidgetTouchCallback& callback ) const
		{
			auto button = ui::Button::create( "guide_01_4.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
			button->setTag( static_cast<int>( tile_type ) );
			button->addTouchEventListener( callback );
			button->setScale9Enabled( true );
			button->setContentSize( menu_size );
			{
				auto label = Label::createWithTTF( button_text, "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				button->setTitleLabel( label );
			}

			return button;
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

			mTileSelectCallback( static_cast<step_pathfinder::game::terrain::eTileType>( sender_node->getTag() ) );
		}
	} // namespace game
}