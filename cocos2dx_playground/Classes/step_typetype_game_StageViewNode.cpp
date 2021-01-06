#include "step_typetype_game_StageViewNode.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"

#include "step_typetype_game_Constant.h"
#include "step_typetype_game_LetterViewNode.h"
#include "step_typetype_game_Stage.h"

USING_NS_CC;

namespace step_typetype
{
	namespace game
	{
		StageViewNode::StageViewNode( const std::size_t max_length ) : mLetters( max_length, nullptr )
		{}

		StageViewNode* StageViewNode::create( const std::size_t max_length, const Config config )
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

		bool StageViewNode::init( const Config config )
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// Content Size
			//
			{
				const auto node_size( CalculateStageSize( mLetters.size() ) );
				setContentSize( node_size );
			}

			//
			// Config Setup
			//
			if( config.bShowPivot )
			{
				auto pivot = Sprite::create( "textures/step_typetype/step_typetype_helper_pivot.png" );
				pivot->setScale( 2.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			{
				auto layer = LayerColor::create( Color4B( 0u, 0u, 0u, 100u ), getContentSize().width, getContentSize().height );
				layer->setPosition( Vec2(
					-getContentSize().width * 0.5f
					, -getContentSize().height * 0.5f
				) );
				addChild( layer, std::numeric_limits<int>::min() );

				if( config.bShowPivot )
				{
					auto pivot = Sprite::create( "textures/step_typetype/step_typetype_helper_pivot.png" );
					pivot->setScale( 2.f );
					layer->addChild( pivot );
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

			const auto stage_size( CalculateStageSize( stage.GetLength() ) );
			const Vec2 head_position( stage_size.width * -0.5f, stage_size.height * -0.5f );
			const Vec2 first_letter_position(
				head_position
				+ Vec2( GameConfig.MarginSize.width, GameConfig.MarginSize.height )
				+ Vec2( ( GameConfig.LetterSize.width * 0.5f ), 0.f )
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
					first_letter_position
					+ Vec2( ( i * GameConfig.LetterSize.width ), 0.f )
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
