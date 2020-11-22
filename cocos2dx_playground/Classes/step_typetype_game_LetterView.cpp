#include "step_typetype_game_LetterView.h"

#include <new>

#include "2d/CCLabel.h"
#include "base/ccUTF8.h"

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
		LetterView::LetterView() : mLabel( nullptr )
		{}

		LetterView* LetterView::create()
		{
			auto ret = new ( std::nothrow ) LetterView();
			if( !ret || !ret->init() )
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

		bool LetterView::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			mLabel = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12 );
			addChild( mLabel );

			return true;
		}

		void LetterView::Reset( const char letter )
		{
			mLabel->setString( StringUtils::format( "%c", letter ) );
			mLabel->setAnchorPoint( Vec2( 0.5f, 0.f ) );
			mLabel->setColor( Color3B::WHITE );
		}
		void LetterView::Die()
		{
			mLabel->setAnchorPoint( Vec2( 0.5f, 1.f ) );
			mLabel->setColor( Color3B( 100u, 100u, 100u ) );
		}
	}
}
