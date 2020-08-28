#pragma once

#include <functional>
#include <vector>

#include "ui/UIWidget.h"

namespace cpgui
{
	class ScrollViewGenerator
	{
	private:
		ScrollViewGenerator() = delete;

	public:
		struct Item
		{
			Item( int tag, const std::string& name ) :
				Tag( tag )
				, Name( name )
			{}

			int Tag = -1;
			std::string Name;
		};
		using ItemContainerT = std::vector<Item>;

		static cocos2d::Node* Create(
			const char* title_string
			, const ItemContainerT& item_container
			, const cocos2d::ui::Widget::ccWidgetTouchCallback& item_select_callback
			, const std::size_t item_visible_count = 20u
			, const cocos2d::Size item_size = cocos2d::Size( 180.f, 20.f )
		);
	};
}
