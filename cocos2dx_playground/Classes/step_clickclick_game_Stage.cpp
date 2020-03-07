#include "step_clickclick_game_Stage.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "ui/UIButton.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		void CheckOddNumber( const int number )
		{
			assert( 1 == ( number & 1 ) );
		}
		void CheckSize( const int pivot, const int number )
		{
			assert( pivot >= number );
		}

		Stage::Stage() :
			mStageWidth( 7 )
			, mStageHeight( 7 )
			, mCenterX( mStageWidth / 2 )
			, mCenterY( mStageWidth / 2 )
			, Pannels()
		{
			//
			// Must odd number
			//
			CheckOddNumber( mStageWidth );
			CheckOddNumber( mStageHeight );
		}

		Stage* Stage::create()
		{
			auto ret = new ( std::nothrow ) Stage();
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

		bool Stage::init()
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
				pivot->setPosition( pivot_position 
);
				addChild( pivot, std::numeric_limits<int>::min() );
			}

			// Buttons
			for( int ty = 0; ty < mStageHeight; ++ty )
			{
				for( int tx = 0; tx < mStageWidth; ++tx )
				{
					const int linear_index = tx + ( ty * mStageWidth );

					auto button = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
					button->setScale9Enabled( true );
					button->setContentSize( tile_size );
					button->setPosition(
						pivot_position
						+ Vec2( tile_size.width * 0.5f, tile_size.height * 0.5f )
						+ Vec2( tx * ( tile_size.width + margin_size.width ), ty * ( tile_size.height + margin_size.height ) )
					);
					addChild( button );
					{
						auto label = Label::createWithTTF( "5", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
						label->setColor( Color3B::RED );
						label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
						button->addChild( label );
						label->setPosition( Vec2(
							label->getParent()->getContentSize().width * 0.5f
							, label->getParent()->getContentSize().height * 0.5f
						) );
					}

					Pannels.emplace_back(
						linear_index
						, 0
						, button
					);
				}
			}

			return true;
		}

		void Stage::Setup( const int width, const int height )
		{
			CheckOddNumber( width );
			CheckOddNumber( height );
			CheckSize( mStageWidth, width );
			CheckSize( mStageHeight, height );

			for( auto p : Pannels )
			{
				p.PannelNode->setVisible( false );
			}

			const int current_pivot_x = mCenterX - ( width / 2 );
			const int current_pivot_y = mCenterY - ( height / 2 );
			for( int ty = current_pivot_y; ty < current_pivot_y + height; ++ty )
			{
				for( int tx = current_pivot_x; tx < current_pivot_x + width; ++tx )
				{
					const int linear_index = tx + ( ty * mStageWidth );

					Pannels[linear_index].PannelNode->setVisible( true );
				}
			}
		}
	} // namespace game
} // namespace step_clickclick
