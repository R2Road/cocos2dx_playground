#include "cpg_ui_ToolBarNode.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "ui/UIButton.h"
#include "ui/UILayout.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_StringTable.h"
#include "cpg_node_PivotNode.h"

USING_NS_CC;

namespace
{
	const int TAG_InputBlock = 10000001;
	const int TAG_InputBlockBG = 10000002;
	const int TAG_ItemIndicator = 20000001;
}

namespace cpg_ui
{
	ToolBarNode::ToolBarNode( const Size item_size, const Size item_margin ) :
		mItemSize( item_size )
		, mItemMargin( item_margin )
		, mLayout( nullptr )
		, mDeselectNode( nullptr )
	{}

	ToolBarNode* ToolBarNode::create(
		const ui::Layout::Type align_type
		, const Size item_size
		, const Size item_margin
	)
	{
		auto ret = new ( std::nothrow ) ToolBarNode( item_size, item_margin );
		if( !ret || !ret->init( align_type ) )
		{
			CC_SAFE_DELETE( ret );
		}
		else
		{
			ret->autorelease();
		}

		return ret;
	}

	bool ToolBarNode::init( const cocos2d::ui::Layout::Type align_type )
	{
		if( !Node::init() )
		{
			return false;
		}

		//
		// Pivot
		//
		{
			addChild( cpg_node::PivotNode::create(), std::numeric_limits<int>::max() );
		}

		//
		// Bar Block
		//
		{
			auto widget = ui::Widget::create();
			widget->setTag( TAG_InputBlock );
			widget->setAnchorPoint( Vec2::ZERO );
			widget->setTouchEnabled( true );
			addChild( widget, std::numeric_limits<int>::min() );

			// Background
			{
				auto background = LayerColor::create( Color4B( 0, 0, 0, 255 ) );
				background->setTag( TAG_InputBlockBG );
				widget->addChild( background );
			}
		}

		//
		// Menus
		//
		{
			mLayout = ui::Layout::create();
			mLayout->setLayoutType( align_type );
			mLayout->setBackGroundColor( Color3B::BLUE );
			mLayout->setBackGroundColorOpacity( 150u );
			addChild( mLayout );
		}

		updateToolBarSize( 1 );

		return true;
	}

	void ToolBarNode::updateToolBarSize( const int tool_count )
	{
		if( ui::Layout::Type::HORIZONTAL == mLayout->getLayoutType() )
		{
			setContentSize( Size(
				( mItemMargin.width + mItemSize.width + mItemMargin.width ) * tool_count
				, mItemMargin.height + mItemSize.height + mItemMargin.height
			) );
		}
		else
		{
			setContentSize( Size(
				mItemMargin.width + mItemSize.width + mItemMargin.width
				, ( mItemMargin.height + mItemSize.height + mItemMargin.height ) * tool_count
			) );
		}

		mLayout->setContentSize( getContentSize() );

		auto input_block = getChildByTag( TAG_InputBlock );
		input_block->setContentSize( getContentSize() );
		input_block->getChildByTag( TAG_InputBlockBG )->setContentSize( getContentSize() );
	}

	void ToolBarNode::AddTool( const int tool_tag, Node* tool_symbol, const std::function<void()>& callback )
	{
		auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
		button->setScale9Enabled( true );
		button->setTag( tool_tag );
		button->setContentSize( mItemSize );
		button->addTouchEventListener( [this, button, callback]( Ref* /*sender*/, ui::Widget::TouchEventType touch_event )
		{
			if( ui::Widget::TouchEventType::ENDED != touch_event )
			{
				return;
			}

			SelectTool( button->getTag() );

			callback();
		} );

		// Pivot
		{
			button->addChild( cpg_node::PivotNode::create(), std::numeric_limits<int>::max() );
		}

		// Align
		{
			auto param = ui::LinearLayoutParameter::create();
			{
				param->setGravity( ui::LinearLayoutParameter::LinearGravity::NONE );
				param->setMargin( ui::Margin( mItemMargin.width, mItemMargin.height, mItemMargin.width, mItemMargin.height ) );
			}

			button->setLayoutParameter( param );
		}

		// Symbol
		{
			tool_symbol->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
			tool_symbol->setPosition(
				button->getContentSize().width * 0.5f
				, button->getContentSize().height * 0.5f
			);
			button->addChild( tool_symbol );
		}

		// Indicator
		{
			auto indicator = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
			indicator->setTag( TAG_ItemIndicator );
			indicator->setScale9Enabled( true );
			indicator->setContentSize( button->getContentSize() );
			indicator->setColor( Color3B::GREEN );
			indicator->setVisible( false );
			indicator->setPosition(
				indicator->getContentSize().width * 0.5f
				, indicator->getContentSize().height * 0.5f
			);
			button->addChild( indicator, std::numeric_limits<int>::max() );
		}

		mLayout->addChild( button );

		updateToolBarSize( mLayout->getChildrenCount() );
	}
	void ToolBarNode::AddTool( const int tool_tag, const char* tool_sprite_frame_path, const std::function<void()>& callback )
	{
		auto tool_sprite = Sprite::createWithSpriteFrameName( tool_sprite_frame_path );
		tool_sprite->setScale( _director->getContentScaleFactor() );
		AddTool( tool_tag, tool_sprite, callback );
	}
	void ToolBarNode::AddTool( const int tool_tag, const char* symbol_string, const int font_size, const std::function<void()>& callback )
	{
		auto label = Label::createWithTTF( symbol_string, cpg::StringTable::GetFontPath(), font_size );
		AddTool( tool_tag, label, callback );
	}

	void ToolBarNode::SelectTool( const int tool_tag )
	{
		for( auto c : mLayout->getChildren() )
		{
			c->getChildByTag( TAG_ItemIndicator )->setVisible( tool_tag == c->getTag() );
		}
	}

	void ToolBarNode::ClearTools()
	{
		mLayout->removeAllChildren();
		updateToolBarSize( 1 );
	}
}