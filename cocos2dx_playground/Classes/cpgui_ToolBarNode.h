#pragma once

#include <functional>

#include "ui/UILayout.h"

namespace cpg_ui
{
	class ToolBarNode : public cocos2d::Node
	{
	private:
		ToolBarNode( const cocos2d::Size item_size, const cocos2d::Size item_margin );

	public:
		static ToolBarNode* create(
			const cocos2d::ui::Layout::Type align_type = cocos2d::ui::Layout::Type::HORIZONTAL
			, const cocos2d::Size item_size = cocos2d::Size( 20.f, 20.f )
			, const cocos2d::Size item_margin = cocos2d::Size( 2.f, 1.f )
		);

	private:
		bool init( const cocos2d::ui::Layout::Type align_type );

		void updateToolBarSize( const int tool_count );

	public:
		void AddTool( const int tool_tag, cocos2d::Node* tool_symbol, const std::function<void()>& callback );
		void AddTool( const int tool_tag, const char* tool_sprite_frame_path, const std::function<void()>& callback );
		void AddTool( const int tool_tag, const char* symbol_string, const int font_size, const std::function<void()>& callback );
		void SelectTool( const int tool_tag );

		void ClearTools();

	private:
		const cocos2d::Size mItemSize;
		const cocos2d::Size mItemMargin;

		cocos2d::ui::Layout* mLayout;
		cocos2d::Node* mDeselectNode;
	};
}
