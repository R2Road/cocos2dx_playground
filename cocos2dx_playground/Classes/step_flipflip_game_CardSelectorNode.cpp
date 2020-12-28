#include "step_flipflip_game_CardSelectorNode.h"

#include <new>
#include <numeric>

#include "audio/include/AudioEngine.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"

#include "step_flipflip_game_Constant.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		CardSelectorNode::CardSelectorNode( const int width, const int height, const Size& card_area_size ) :
			mWidth( width )
			, mHeight( height )
			, mCardAreaSize( card_area_size )
			, mPivotPosition( card_area_size.width * 0.5f, card_area_size.height * 0.5f )

			, mCurrentX( -1 )
			, mCurrentY( -1 )
			, mIndicator( nullptr )
		{}

		CardSelectorNode* CardSelectorNode::create( const StageConfig& stage_config, const bool show_guide )
		{
			auto ret = new ( std::nothrow ) CardSelectorNode( stage_config.Width, stage_config.Height, stage_config.CardAreaSize );
			if( !ret || !ret->init( show_guide ) )
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

		bool CardSelectorNode::init( const bool show_guide )
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// Content Size
			//
			setContentSize( Size( mCardAreaSize.width * mWidth, mCardAreaSize.height * mHeight ) );

			//
			// Debug Guide
			//
			if( show_guide )
			{
				auto layer = LayerColor::create( Color4B::BLACK, getContentSize().width, getContentSize().height );
				addChild( layer, std::numeric_limits<int>::min() );
			}

			//
			// Build
			//
			{
				mIndicator = Sprite::createWithSpriteFrameName( step_flipflip::game::GetSpriteFrameName_CardIndicator() );
				addChild( mIndicator );
			}

			//
			// Setup
			//
			MoveIndicator( 0, 0, false );

			return true;
		}

		void CardSelectorNode::MoveIndicator( const int move_amount_x, const int move_amount_y, const bool bPlay_SFX )
		{
			const auto last_x = mCurrentX;
			const auto last_y = mCurrentY;

			mCurrentX = std::max(
				std::min( mCurrentX + move_amount_x, mWidth - 1 )
				, 0
			);
			mCurrentY = std::max(
				std::min( mCurrentY + move_amount_y, mHeight - 1 )
				, 0
			);

			if( last_x != mCurrentX || last_y != mCurrentY )
			{
				mIndicator->setPosition(
					mPivotPosition
					+ Vec2( mCardAreaSize.width * mCurrentX, mCardAreaSize.height * mCurrentY )
				);

				if( bPlay_SFX )
				{
					experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );
				}
			}
			else
			{
				if( bPlay_SFX )
				{
					experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.1f );
				}
			}
		}
	}
}
