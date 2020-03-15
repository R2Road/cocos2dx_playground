#include "step_typetype_game_StageView.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace step_typetype
{
	namespace game
	{
		StageView::StageView( const std::size_t max_length ) :
			mLetters( max_length, nullptr )
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

			const Size letter_size( 12.f, 12.f );
			const Size margin_size( 2.f, 2.f );
			const Size stage_size(
				margin_size.width
				+ ( mLetters.size() * letter_size.width )
				+ margin_size.width
				, margin_size.height
				+ letter_size.height
			);
			const Vec2 pivot_position( stage_size.width * -0.5f, stage_size.height * -0.5f );

			setContentSize( stage_size );

			if( config.bShowPivot )
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 2.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			if( config.bShowBackgroundGuide )
			{
				auto background = LayerColor::create( Color4B( 0u, 0u, 0u, 100u ), getContentSize().width, getContentSize().height );
				background->setPosition( pivot_position );
				addChild( background, std::numeric_limits<int>::min() );
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 2.f );
					background->addChild( pivot );
				}
			}

			const Vec2 letter_pivot_position(
				pivot_position
				+ Vec2( margin_size.width, margin_size.height )
				+ Vec2( ( letter_size.width * 0.5f ), 0.f )
			);
			int letter_index = 0;
			for( auto& letter : mLetters )
			{
				letter = Label::createWithTTF( "A", "fonts/arial.ttf", 10 );
				letter->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				letter->setPosition(
					letter_pivot_position
					+ Vec2( ( letter_index * letter_size.width ), 0.f )
				);
				addChild( letter );

				++letter_index;
			}

			return true;
		}
	}
}
