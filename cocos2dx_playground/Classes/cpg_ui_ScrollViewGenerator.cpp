#include "cpg_ui_ScrollViewGenerator.h"

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIScrollView.h"

#include "cpg_StringTable.h"

USING_NS_CC;

namespace cpg_ui
{
	Node* CreateScrollViewItem( const ScrollViewGenerator::Config& config, const cpg_ui::ScrollViewGenerator::Item& item_info, const ui::Widget::ccWidgetTouchCallback& item_select_callback )
	{
		auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
		button->setTag( item_info.Tag );
		button->setScale9Enabled( true );
		button->setContentSize( config.ItemSize );
		button->addTouchEventListener( item_select_callback );

		// Label
		{
			auto label = Label::createWithTTF( item_info.Name, cpg::StringTable::GetFontPath(), config.FontSize );
			label->getFontAtlas()->setAliasTexParameters();
			button->setTitleLabel( label );
		}

		// Align
		{
			auto param = ui::LinearLayoutParameter::create();
			param->setMargin( config.ItemMargin );

			button->setLayoutParameter( param );
		}

		// Pivot
		{
			auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
			button->addChild( pivot, std::numeric_limits<int>::max() );
		}

		// Indicator
		if( config.bAddItemIndicator )
		{
			auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_4.png" );
			sprite->setTag( ScrollViewGenerator::eTAG::Indicator );
			sprite->setAnchorPoint( Vec2::ZERO );
			sprite->setContentSize( config.ItemSize );
			sprite->setVisible( false );
			button->addChild( sprite, 1 );
		}

		return button;
	}

	Node* ScrollViewGenerator::Create(
		const Config& config
		, const char* title_string
		, const ItemContainerT& item_info_container
		, const ui::Widget::ccWidgetTouchCallback& item_select_callback
	)
	{
		const auto visible_button_count = std::max( 1u, config.ItemVisibleCount );

		const Size ListVisibleSize(
			config.ItemMargin.left + config.ItemSize.width + config.ItemMargin.right
			, ( config.ItemMargin.top + config.ItemSize.height + config.ItemMargin.bottom ) * visible_button_count
		);

		const Size ListTotalSize(
			config.ItemMargin.left + config.ItemSize.width + config.ItemMargin.right
			, ( config.ItemMargin.top + config.ItemSize.height + config.ItemMargin.bottom ) * std::max( visible_button_count, item_info_container.size() )
		);

		auto root_node = Node::create();
		root_node->setContentSize( ListVisibleSize );
		{
			auto scroll_view = ui::ScrollView::create();
			scroll_view->setTag( ScrollViewGenerator::eTAG::ScrollView );
			scroll_view->setDirection( ui::ScrollView::Direction::VERTICAL );
			scroll_view->setContentSize( ListVisibleSize );
			scroll_view->setInnerContainerSize( ListTotalSize );
			root_node->addChild( scroll_view );
			{
				auto layout_node = ui::Layout::create();
				layout_node->setTag( ScrollViewGenerator::eTAG::Layout );
				layout_node->setContentSize( ListTotalSize );
				layout_node->setLayoutType( ui::Layout::Type::VERTICAL );
				layout_node->setBackGroundColor( Color3B( 0, 148, 255 ) );
				layout_node->setBackGroundColorOpacity( 150u );
				layout_node->setBackGroundColorType( cocos2d::ui::Layout::BackGroundColorType::SOLID );
				scroll_view->addChild( layout_node );
				{
					for( auto cur = item_info_container.begin(), end = item_info_container.end(); end != cur; ++cur )
					{
						auto button = CreateScrollViewItem( config, *cur, item_select_callback );
						layout_node->addChild( button );
					}
				}
			}
		}

		//
		// Title Label
		//
		{
			auto label = Label::createWithTTF( title_string, cpg::StringTable::GetFontPath(), 9 );
			label->getFontAtlas()->setAliasTexParameters();
			label->setAnchorPoint( Vec2::ZERO );
			label->setPositionY( root_node->getContentSize().height );
			root_node->addChild( label );
		}

		return root_node;
	}
}