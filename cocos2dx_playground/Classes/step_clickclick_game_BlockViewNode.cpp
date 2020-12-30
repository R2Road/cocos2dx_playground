#include "step_clickclick_game_BlockViewNode.h"

#include <new>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/ccMacros.h"
#include "ui/UIButton.h"

#include "cpg_StringTable.h"

#include "step_clickclick_game_EffectView.h"
#include "step_clickclick_game_Stage.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		BlockViewNode::BlockViewNode( const int index, const OnBlockCallback& on_block_callback ) :
			mIndex( index )

			, mButtonNode( nullptr )
			, mViewNode( nullptr )
			, mLifeLabel( nullptr )
			, mEffectNode( nullptr )

			, mOnBlockCallback( on_block_callback )
		{}

		BlockViewNode* BlockViewNode::create( const int index, const cocos2d::Size block_size, const OnBlockCallback& on_block_callback )
		{
			auto ret = new ( std::nothrow ) BlockViewNode( index, on_block_callback );
			if( !ret || !ret->init( block_size ) )
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

		bool BlockViewNode::init( const cocos2d::Size block_size )
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// button
			//
			{
				auto button = ui::Button::create( "guide_empty.png", "guide_empty.png", "guide_empty.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->setContentSize( block_size );
				button->addTouchEventListener( CC_CALLBACK_2( BlockViewNode::onBlock, this ) );
				addChild( button );

				mButtonNode = button;
			}

			//
			// view
			//
			{
				mViewNode = Sprite::create();
				mViewNode->setScale( _director->getContentScaleFactor() );
				addChild( mViewNode, 1 );
			}

			//
			// label
			//
			{
				mLifeLabel = Label::createWithTTF( "0", cpg::StringTable::GetFontPath(), 10 );
				mLifeLabel->getFontAtlas()->setAliasTexParameters();
				addChild( mLifeLabel, 2 );
			}

			//
			// effect
			//
			{
				mEffectNode = EffectView::create();
				addChild( mEffectNode, 3 );
			}

			return true;
		}

		void BlockViewNode::Reset( eBlockType type, const int life )
		{
			mLifeLabel->setString( std::to_string( life ) );

			SpriteFrame* view_frame = nullptr;
			switch( type )
			{
			case eBlockType::Single:
				view_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_block_single.png" );
				break;
			case eBlockType::Same:
				view_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_block_together.png" );
				break;
			case eBlockType::Different:
				view_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_block_different.png" );
				break;
			default:
				CCASSERT( false, "Invalid Block Type" );
			}
			mViewNode->setSpriteFrame( view_frame );
		}
		void BlockViewNode::SetVisible( const bool visible )
		{
			mButtonNode->setVisible( visible );
			mViewNode->setVisible( visible );
			mLifeLabel->setVisible( visible );
		}
		void BlockViewNode::UpdateLife( const int last_life, const int current_life )
		{
			if( 0 == current_life )
			{
				SetVisible( false );
				mEffectNode->PlayEffect( EffectView::eAnimationIndex::Die );
			}
			else
			{
				mLifeLabel->setString( std::to_string( current_life ) );

				if( last_life < current_life )
				{
					mEffectNode->PlayEffect( EffectView::eAnimationIndex::Increase );
				}
				else
				{
					mEffectNode->PlayEffect( EffectView::eAnimationIndex::Decrease );
				}
			}
		}

		void BlockViewNode::onBlock( Ref* /*sender*/, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				mOnBlockCallback( mIndex );
			}
		}
	}
}
