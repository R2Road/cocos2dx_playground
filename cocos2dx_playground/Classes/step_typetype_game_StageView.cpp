#include "step_typetype_game_StageView.h"

#include <algorithm>
#include <new>
#include <numeric>
#include <string>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/ccUTF8.h"

#include "step_typetype_game_Stage.h"
#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace
{
	const Size letter_size( 12.f, 12.f );
	const Size margin_size( 2.f, 2.f );
}

namespace step_typetype
{
	namespace game
	{
		StageView::StageView( const std::size_t max_length ) :
			mLetters( max_length, nullptr )
			, mIndicator( nullptr )
		{}

		StageView* StageView::create( const std::size_t max_length, const StageViewConfig config )
		{
			auto ret = new ( std::nothrow ) StageView( max_length );
			if( !ret || !ret->init( config ) )
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

		bool StageView::init( const StageViewConfig config )
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// Content Size
			//
			{
				const Size node_size(
					margin_size.width
					+ ( mLetters.size() * letter_size.width )
					+ margin_size.width
					, margin_size.height
					+ letter_size.height
				);
				setContentSize( node_size );
			}

			//
			// Config Setup
			//
			{
				if( config.bShowPivot )
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 2.f );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				if( config.bShowBackgroundGuide )
				{
					auto background = LayerColor::create( Color4B( 0u, 0u, 0u, 100u ), getContentSize().width, getContentSize().height );
					background->setPosition( Vec2(
						-getContentSize().width * 0.5f
						, 1 -getContentSize().height * 0.5f
					) );
					addChild( background, std::numeric_limits<int>::min() );

					if( config.bShowPivot )
					{
						auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
						pivot->setScale( 2.f );
						background->addChild( pivot );
					}
				}
			}

			//
			// Letters
			//
			for( auto& letter : mLetters )
			{
				letter = Label::createWithTTF( "A", "fonts/NanumSquareR.ttf", 10 );
				letter->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				addChild( letter );
			}

			//
			// Indicator
			//
			{
				mIndicator = Sprite::createWithSpriteFrameName( "guide_01_4.png" );
				mIndicator->setContentSize( Size( 12.f, 12.f ) );
				mIndicator->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				addChild( mIndicator );
			}

			return true;
		}

		void StageView::Reset( const Stage& stage )
		{
			assert( stage.GetLength() <= mLetters.size() );

			//
			// Hide All
			//
			for( auto l : mLetters )
			{
				l->setVisible( false );
			}

			const Size stage_size(
				margin_size.width
				+ ( stage.GetLength() * letter_size.width )
				+ margin_size.width
				, margin_size.height
				+ letter_size.height
			);
			const Vec2 pivot_position( stage_size.width * -0.5f, stage_size.height * -0.5f );
			const Vec2 letter_pivot_position(
				pivot_position
				+ Vec2( margin_size.width, margin_size.height )
				+ Vec2( ( letter_size.width * 0.5f ), 0.f )
			);

			//
			// Setup Letters
			//
			for( std::size_t i = 0; i < stage.GetLength(); ++i )
			{
				mLetters[i]->setString( StringUtils::format( "%c", stage.GetLetter( i ) ) );
				mLetters[i]->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				mLetters[i]->setPosition(
					letter_pivot_position
					+ Vec2( ( i * letter_size.width ), 0.f )
				);
				mLetters[i]->setColor( Color3B::WHITE );
				mLetters[i]->setVisible( true );
			}

			//
			// Setup Indicator
			//
			{
				mIndicator->setPosition( mLetters[0u]->getPosition() );
				mIndicator->setVisible( true );
			}
		}
		void StageView::RequestLetterDie( const std::size_t target_pos )
		{
			assert( mLetters.size() > target_pos );

			mLetters[target_pos]->setAnchorPoint( Vec2( 0.5f, 1.f ) );
			mLetters[target_pos]->setColor( Color3B( 100u, 100u, 100u ) );

			const auto indicator_pos = target_pos + 1u;
			if( mLetters.size() > indicator_pos && mLetters[indicator_pos]->isVisible() )
			{
				mIndicator->setPosition( mLetters[indicator_pos]->getPosition() );
			}
			else
			{
				mIndicator->setVisible( false );
			}
		}
	}
}
