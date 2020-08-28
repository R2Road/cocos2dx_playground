#include "cpgui_ScrollViewGenerator.h"

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "ui/UIButton.h"
#include "ui/UIScrollView.h"

USING_NS_CC;

namespace
{
	const char* FontPath = "fonts/NanumSquareR.ttf";
}

namespace cpgui
{
	Node* CreateScrollViewItem( cocos2d::Size item_size, cocos2d::Size button_margin, const cpgui::ScrollViewGenerator::Item& item_info, const ui::Widget::ccWidgetTouchCallback& item_select_callback )
	{
		auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
		button->setTag( item_info.Tag );
		button->setScale9Enabled( true );
		button->setContentSize( item_size );
		button->addTouchEventListener( item_select_callback );

		// Label
		{
			auto label = Label::createWithTTF( item_info.Name, FontPath, 9 );
			label->getFontAtlas()->setAliasTexParameters();
			button->setTitleLabel( label );
		}

		// Align
		{
			auto param = ui::LinearLayoutParameter::create();
			auto margin = ui::Margin( button_margin.width, button_margin.height, button_margin.width, button_margin.height );
			param->setMargin( margin );

			button->setLayoutParameter( param );
		}

		// Pivot
		{
			auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
			button->addChild( pivot, std::numeric_limits<int>::max() );
		}

		return button;
	}

	Node* ScrollViewGenerator::Create(
		const char* title_string
		, const ItemContainerT& item_info_container
		, const ui::Widget::ccWidgetTouchCallback& item_select_callback
		, const std::size_t item_visible_count
		, const Size item_size
	)
	{
		const auto visible_button_count = std::max( 1u, item_visible_count );

		const Size ListInnerMargin( 1, 1 );
		const Size ListVisibleSize(
			ListInnerMargin.width + item_size.width + ListInnerMargin.width
			, ( ListInnerMargin.height + item_size.height + ListInnerMargin.height ) * visible_button_count
		);

		const Size ListTotalSize(
			ListInnerMargin.width + item_size.width + ListInnerMargin.width
			, ( ListInnerMargin.height + item_size.height + ListInnerMargin.height ) * std::max( visible_button_count, item_info_container.size() )
		);

		const Size RootMargin( 2, 2 );
		const Size RootSize(
			RootMargin
			+ ListVisibleSize
			+ RootMargin
		);

		auto root_node = Node::create();
		root_node->setContentSize( ListVisibleSize );
		{
			auto scroll_view = ui::ScrollView::create();
			scroll_view->setDirection( ui::ScrollView::Direction::VERTICAL );
			scroll_view->setContentSize( ListVisibleSize );
			scroll_view->setInnerContainerSize( ListTotalSize );
			root_node->addChild( scroll_view );
			{
				auto layout_node = ui::Layout::create();
				layout_node->setContentSize( ListTotalSize );
				layout_node->setLayoutType( ui::Layout::Type::VERTICAL );
				layout_node->setBackGroundColor( Color3B( 0, 148, 255 ) );
				layout_node->setBackGroundColorOpacity( 150u );
				layout_node->setBackGroundColorType( cocos2d::ui::Layout::BackGroundColorType::SOLID );
				scroll_view->addChild( layout_node );
				{
					for( auto cur = item_info_container.begin(), end = item_info_container.end(); end != cur; ++cur )
					{
						auto button = CreateScrollViewItem( item_size, ListInnerMargin, *cur, item_select_callback );
						layout_node->addChild( button );
					}
				}
			}
		}

		//
		// Title Label
		//
		{
			auto label = Label::createWithTTF( title_string, FontPath, 9 );
			label->getFontAtlas()->setAliasTexParameters();
			label->setAnchorPoint( Vec2::ZERO );
			label->setPositionY( root_node->getContentSize().height );
			root_node->addChild( label );
		}

		return root_node;
	}
}