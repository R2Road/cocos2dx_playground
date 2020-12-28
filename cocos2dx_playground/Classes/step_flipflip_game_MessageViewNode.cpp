#include "step_flipflip_game_MessageViewNode.h"

#include <new>
#include <numeric>

#include "2d/CCActionInterval.h"
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
			mLayer( nullptr )
			, mLabel( nullptr )
			, mBlinkAction( nullptr )
		{}
		MessageViewNode::~MessageViewNode()
		{
			mBlinkAction->release();
		}

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
				mLayer = LayerColor::create( Color4B::BLACK, visibleSize.width, 50.f );
				mLayer->setPosition( -mLayer->getContentSize().width * 0.5f, -mLayer->getContentSize().height * 0.5f );
				mLayer->setCascadeOpacityEnabled( true );
				addChild( mLayer, std::numeric_limits<int>::min() );
			}

			//
			// Label
			//
			{
				mLabel = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 10 );
				mLabel->setPosition( mLayer->getContentSize().width * 0.5f, mLayer->getContentSize().height * 0.5f );
				mLayer->addChild( mLabel );
			}

			//
			// Blink Action
			//
			{
				auto fadeInAction = FadeIn::create( 0.6f );
				auto fadeInkDelay = DelayTime::create( 0.4f );
				auto fadeOutAction = FadeOut::create( 0.8f );
				auto fadeOutkDelay = DelayTime::create( 0.2f );
				mBlinkAction = Sequence::create( fadeInAction, fadeInkDelay, fadeOutAction, fadeOutkDelay, nullptr );
				mBlinkAction->retain();
			}

			//
			// Setup
			//
			mLayer->setOpacity( 0u );

			return true;
		}

		
		bool MessageViewNode::isMessaging() const
		{
			return 0u < mLayer->getNumberOfRunningActions();
		}
		void MessageViewNode::ShowMessage( const char* str )
		{
			if( isMessaging() )
			{
				return;
			}

			mLabel->setString( str );
			mLayer->runAction( mBlinkAction );
		}
	}
}
