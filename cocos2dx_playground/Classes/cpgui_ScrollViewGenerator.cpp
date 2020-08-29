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
	Node* CreateScrollViewItem( const ScrollViewGenerator::Config& config, const cpgui::ScrollViewGenerator::Item& item_info, const ui::Widget::ccWidgetTouchCallback& item_select_callback )
	{
		auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
		button->setTag( item_info.Tag );
		button->setScale9Enabled( true );
		button->setContentSize( config.ItemSize );
		button->addTouchEventListener( item_select_callback );

		// Label
		{
			auto label = Label::createWithTTF( item_info.Name, FontPath, config.FontSize );
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
			auto label = Label::createWithTTF( title_string, FontPath, 9 );
			label->getFontAtlas()->setAliasTexParameters();
			label->setAnchorPoint( Vec2::ZERO );
			label->setPositionY( root_node->getContentSize().height );
			root_node->addChild( label );
		}

		return root_node;
	}
}