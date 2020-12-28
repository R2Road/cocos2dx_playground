#include "step_flipflip_game_MessageViewNode.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "audio/include/AudioEngine.h"
#include "base/CCDirector.h"

#include "step_flipflip_game_Constant.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		MessageViewNode::MessageViewNode() :
			mLabel( nullptr )
		{}

		MessageViewNode* MessageViewNode::create()
		{
			auto ret = new ( std::nothrow ) MessageViewNode();
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

		bool MessageViewNode::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();

			//
			// Background
			//
			{
				auto layer = LayerColor::create( Color4B( 0, 0, 0, 50 ), visibleSize.width, 50.f );
				layer->setPosition( -layer->getContentSize().width * 0.5f, -layer->getContentSize().height * 0.5f );
				addChild( layer, std::numeric_limits<int>::min() );
			}

			//
			// Label
			//
			{
				mLabel = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 10 );
				addChild( mLabel );
			}

			return true;
		}

		
		void MessageViewNode::ShowMessage( const char* str )
		{
			mLabel->setString( str );
		}
	}
}
