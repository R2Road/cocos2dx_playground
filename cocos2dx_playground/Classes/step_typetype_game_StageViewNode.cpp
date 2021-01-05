#include "step_typetype_game_StageViewNode.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"

#include "step_typetype_game_LetterViewNode.h"
#include "step_typetype_game_Stage.h"

USING_NS_CC;

namespace
{
	const Size letter_size( 14.f, 14.f );
	const Size margin_size( 2.f, 2.f );
}

namespace step_typetype
{
	namespace game
	{
		StageViewNode::StageViewNode( const std::size_t max_length ) : mLetters( max_length, nullptr )
		{}

		StageViewNode* StageViewNode::create( const std::size_t max_length, const StageViewConfig config )
		{
			auto ret = new ( std::nothrow ) StageViewNode( max_length );
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

		bool StageViewNode::init( const StageViewConfig config )
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
					auto pivot = Sprite::create( "textures/step_typetype/step_typetype_helper_pivot.png" );
					pivot->setScale( 2.f );
					addChild( pivot, std::numeric_limits<int>::max() );
				}

				if( config.bShowBackgroundGuide )
				{
					auto background = LayerColor::create( Color4B( 0u, 0u, 0u, 100u ), getContentSize().width, getContentSize().height );
					background->setPosition( Vec2(
						-getContentSize().width * 0.5f
						, -getContentSize().height * 0.5f
					) );
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
			// Letters
			//
			for( auto& letter : mLetters )
			{
				letter = LetterViewNode::create();
				addChild( letter );
			}

			return true;
		}

		void StageViewNode::Reset( const Stage& stage )
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
				+ margin_size.height
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
			LetterViewNode* letter_view_node = nullptr;
			for( std::size_t i = 0; i < stage.GetLength(); ++i )
			{
				letter_view_node = mLetters[i];

				letter_view_node->setVisible( true );
				letter_view_node->Reset( stage.GetLetter( i ) );
				letter_view_node->setPosition(
					letter_pivot_position
					+ Vec2( ( i * letter_size.width ), 0.f )
				);
			}
		}
		void StageViewNode::RequestLetterDie( const std::size_t target_pos )
		{
			CCASSERT( mLetters.size() > target_pos, "Do Range Check : StageViewNode::RequestLetterDie" );

			//
			// Setup Letter
			//
			mLetters[target_pos]->Die();
		}
	}
}
