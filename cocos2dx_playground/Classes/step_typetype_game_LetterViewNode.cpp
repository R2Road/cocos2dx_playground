#include "step_typetype_game_LetterViewNode.h"

#include <new>

#include "2d/CCLabel.h"
#include "base/ccUTF8.h"

USING_NS_CC;

namespace step_typetype
{
	namespace game
	{
		LetterViewNode::LetterViewNode() : mLabel( nullptr )
		{}

		LetterViewNode* LetterViewNode::create()
		{
			auto ret = new ( std::nothrow ) LetterViewNode();
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

		bool LetterViewNode::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			mLabel = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12 );
			addChild( mLabel );

			return true;
		}

		void LetterViewNode::Reset( const char letter )
		{
			mLabel->setString( StringUtils::format( "%c", letter ) );
			mLabel->setAnchorPoint( Vec2( 0.5f, 0.f ) );
			mLabel->setColor( Color3B::WHITE );
		}
		void LetterViewNode::Die()
		{
			mLabel->setAnchorPoint( Vec2( 0.5f, 1.f ) );
			mLabel->setColor( Color3B( 100u, 100u, 100u ) );
		}
	}
}
