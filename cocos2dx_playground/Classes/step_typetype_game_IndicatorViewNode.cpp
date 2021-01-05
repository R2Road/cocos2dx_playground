#include "step_typetype_game_IndicatorViewNode.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"

#include "step_typetype_game_Constant.h"

USING_NS_CC;

namespace
{
	const int TAG_Background = 20140416;
}

namespace step_typetype
{
	namespace game
	{
		IndicatorViewNode::IndicatorViewNode() :
			mIndicator( nullptr )
		{}

		IndicatorViewNode* IndicatorViewNode::create( const Config config )
		{
			auto ret = new ( std::nothrow ) IndicatorViewNode();
			if( !ret || !ret->init( config ) )
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

		bool IndicatorViewNode::init( const Config config )
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// Config Setup
			//
			{
				if( config.bShowPivot )
				{
					auto pivot = Sprite::create( "textures/step_typetype/step_typetype_helper_pivot.png" );
					pivot->setScale( 2.f );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				if( config.bShowBackgroundGuide )
				{
					auto background = LayerColor::create( Color4B( 0u, 0u, 0u, 100u ), getContentSize().width, getContentSize().height );
					background->setTag( TAG_Background );
					addChild( background, std::numeric_limits<int>::min() );

					if( config.bShowPivot )
					{
						auto pivot = Sprite::create( "textures/step_typetype/step_typetype_helper_pivot.png" );
						pivot->setScale( 2.f );
						background->addChild( pivot );
					}
				}
			}

			//
			// Indicator
			//
			{
				mIndicator = Sprite::create( "textures/step_typetype/step_typetype_letter_guide.png" );
				mIndicator->setContentSize( GameConfig.LetterSize );
				mIndicator->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				addChild( mIndicator );
			}

			return true;
		}

		void IndicatorViewNode::Reset( const std::size_t current_stage_length )
		{
			const Size stage_size(
				GameConfig.MarginSize.width
				+ ( current_stage_length * GameConfig.LetterSize.width )
				+ GameConfig.MarginSize.width
				, GameConfig.MarginSize.height
				+ GameConfig.LetterSize.height
				+ GameConfig.MarginSize.height
			);
			setContentSize( stage_size );

			auto background = getChildByTag( TAG_Background );
			if( background )
			{
				background->setContentSize( getContentSize() );
				background->setPosition( Vec2(
					-getContentSize().width * 0.5f
					, -getContentSize().height * 0.5f
				) );
			}

			SetIndicatorPosition( 0u );
		}
		void IndicatorViewNode::SetIndicatorPosition( const std::size_t target_pos )
		{
			const Vec2 pivot_position( getContentSize().width * -0.5f, getContentSize().height * -0.5f );
			const Vec2 letter_pivot_position(
				pivot_position
				+ Vec2( GameConfig.MarginSize.width, GameConfig.MarginSize.height )
				+ Vec2( GameConfig.LetterSize.width * 0.5f, 0.f )
			);

			//
			// Setup Indicator
			//
			{
				mIndicator->setPosition(
					letter_pivot_position
					+ Vec2( target_pos * GameConfig.LetterSize.width, 0.f )
				);
				mIndicator->setVisible( true );
			}
		}
	}
}
