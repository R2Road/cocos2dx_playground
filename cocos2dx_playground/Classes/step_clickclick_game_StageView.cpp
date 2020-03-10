#include "step_clickclick_game_StageView.h"

#include <functional>
#include <new>
#include <numeric>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/ccMacros.h"
#include "ui/UIButton.h"
#include "2d/CCSpriteFrameCache.h"

#include "step_clickclick_game_Stage.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		namespace
		{
			void CheckOddNumber( const int number )
			{
				assert( 1 == ( number & 1 ) );
			}
		}

		StageView::PannelView::PannelView( cocos2d::Node* const pannel_node, cocos2d::Sprite* const view_node, cocos2d::Label* const label_node
			, cocos2d::Sprite* const increase_effect_node, cocos2d::Sprite* const decrease_effect_node ) :
			mPannelNode( pannel_node )
			, mViewNode( view_node )
			, mLabelNode( label_node )
			, mIncreaseEffectNode( increase_effect_node )
			, mDecreaseEffectNode( decrease_effect_node )
		{}
		void StageView::PannelView::Init( ePannelType type, const int life )
		{
			mLabelNode->setString( std::to_string( life ) );

			SpriteFrame* view_frame = nullptr;
			switch( type )
			{
			case ePannelType::Single:
				view_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_pannel_single.png" );
				break;
			case ePannelType::Same:
				view_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_pannel_together.png" );
				break;
			case ePannelType::Different:
				view_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_pannel_different.png" );
				break;
			default:
				assert( false );
			}
			mViewNode->setSpriteFrame( view_frame );
		}
		void StageView::PannelView::SetVisible( const bool visible )
		{
			mPannelNode->setVisible( visible );
			mViewNode->setVisible( visible );
			mLabelNode->setVisible( visible );
		}
		void StageView::PannelView::Update( const int last_life, const int current_life )
		{
			if( 0 == current_life )
			{
				SetVisible( false );
			}
			else
			{
				mLabelNode->setString( std::to_string( current_life ) );

				if( last_life < current_life )
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.1f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_increase1.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_increase2.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_increase3.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_increase4.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_increase5.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "empty_2x2.png" ) );

					auto animate_action = Animate::create( animation_object );

					mIncreaseEffectNode->stopAllActions();
					mIncreaseEffectNode->runAction( animate_action );
				}
				else
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.1f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_decrease1.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_decrease2.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_decrease3.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_decrease4.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_effect_decrease5.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "empty_2x2.png" ) );

					auto animate_action = Animate::create( animation_object );

					mIncreaseEffectNode->stopAllActions();
					mIncreaseEffectNode->runAction( animate_action );
				}
			}
		}



		StageView::StageView( const int width, const int height, const OnPannelCallback& on_pannel_callback ) :
			mStageWidth( width )
			, mStageHeight( height )
			, mGridIndexConverter( mStageWidth, mStageHeight )
			, PannelViews()
			, mOnPannelCallback( on_pannel_callback )
		{
			//
			// Must odd number
			//
			CheckOddNumber( mStageWidth );
			CheckOddNumber( mStageHeight );
		}

		StageView* StageView::create( const int width, const int height, const OnPannelCallback& on_pannel_callback )
		{
			auto ret = new ( std::nothrow ) StageView( width, height, on_pannel_callback );
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

		bool StageView::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			const Size tile_size( 32.f, 32.f );
			const Size margin_size( 2.f, 2.f );
			const Size stage_size(
				( mStageWidth * tile_size.width ) + ( ( mStageWidth - 1 ) * margin_size.width )
				,( mStageHeight * tile_size.height ) + ( ( mStageHeight - 1 ) * margin_size.height )
			);
			const Vec2 pivot_position( stage_size.width * -0.5f, stage_size.height * -0.5f );

			setContentSize( stage_size );

			// Pivot
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 2.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			// Background Guide
			{
				auto pivot = LayerColor::create( Color4B( 0u, 0u, 0u, 100u ), getContentSize().width, getContentSize().height );
				pivot->setPosition( pivot_position );
				addChild( pivot, std::numeric_limits<int>::min() );
			}

			// Buttons
			int linear_index = 0;
			for( int ty = 0; ty < mStageHeight; ++ty )
			{
				for( int tx = 0; tx < mStageWidth; ++tx )
				{
					linear_index = mGridIndexConverter.To_Linear( tx, ty );

					// button
					auto button = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
					button->setTag( linear_index );
					button->setScale9Enabled( true );
					button->setContentSize( tile_size );
					button->setPosition(
						pivot_position
						+ Vec2( tile_size.width * 0.5f, tile_size.height * 0.5f )
						+ Vec2( tx * ( tile_size.width + margin_size.width ), ty * ( tile_size.height + margin_size.height ) )
					);
					button->addTouchEventListener( CC_CALLBACK_2( StageView::onPannel, this ) );
					addChild( button );

					// view
					auto view_node = Sprite::create();
					view_node->setScale( 2.f );
					view_node->setPosition( button->getPosition() );
					addChild( view_node, 1 );

					// label
					auto label = Label::createWithTTF( "0", "fonts/arial.ttf", 9 );
					label->getFontAtlas()->setAliasTexParameters();
					label->setColor( Color3B::WHITE );
					label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
					label->setPosition( button->getPosition() );
					addChild( label, 2 );

					// effect
					auto increase_effect_node = Sprite::create();
					increase_effect_node->setScale( 2.f );
					increase_effect_node->setPosition( button->getPosition() );
					addChild( increase_effect_node, 3 );

					auto decrease_effect_node = Sprite::create();
					decrease_effect_node->setScale( 2.f );
					decrease_effect_node->setPosition( button->getPosition() );
					addChild( decrease_effect_node, 3 );

					PannelViews.emplace_back(
						button
						, view_node
						, label
						, increase_effect_node
						, decrease_effect_node
					);
				}
			}

			return true;
		}

		void StageView::Setup( const Stage& stage_data )
		{
			assert( mStageWidth == stage_data.GetWidth() );
			assert( mStageHeight == stage_data.GetHeight() );

			for( auto& p : PannelViews )
			{
				p.SetVisible( false );
			}

			for( const auto& p : stage_data.GetPannelDatas() )
			{
				if( !p.IsActive() )
				{
					continue;
				}

				PannelViews[p.GetIndex()].Init( p.GetType(), p.GetLife() );
				PannelViews[p.GetIndex()].SetVisible( true );
			}
		}

		void StageView::UpdatePannel( const int linear_index, const int last_life, const int current_life )
		{
			PannelViews[linear_index].Update( last_life, current_life );
		}

		void StageView::onPannel( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			auto button_node = static_cast<Node*>( sender );
			mOnPannelCallback( button_node->getTag() );
		}
	} // namespace game
} // namespace step_clickclick
