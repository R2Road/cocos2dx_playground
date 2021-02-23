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
			, mFadeInOutAction( nullptr )
		{}
		MessageViewNode::~MessageViewNode()
		{
			mFadeInOutAction->release();
		}

		MessageViewNode* MessageViewNode::create( const Color4B& background_color )
		{
			auto ret = new ( std::nothrow ) MessageViewNode();
			if( !ret || !ret->init( background_color ) )
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

		bool MessageViewNode::init( const Color4B& background_color )
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
				mLayer = LayerColor::create( background_color, visibleSize.width, 40.f );
				mLayer->setPosition( -mLayer->getContentSize().width * 0.5f, -mLayer->getContentSize().height * 0.5f );
				mLayer->setCascadeOpacityEnabled( true );
				addChild( mLayer, std::numeric_limits<int>::min() );
			}

			//
			// Label
			//
			{
				mLabel = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 14 );
				mLabel->setPosition( mLayer->getContentSize().width * 0.5f, mLayer->getContentSize().height * 0.5f );
				mLayer->addChild( mLabel );
			}

			//
			// Blink Action
			//
			{
				auto fadeInAction = FadeIn::create( 0.4f );
				auto fadeInkDelay = DelayTime::create( 1.5f );
				auto fadeOutAction = FadeOut::create( 0.1f );
				mFadeInOutAction = Sequence::create( fadeInAction, fadeInkDelay, fadeOutAction, nullptr );
				mFadeInOutAction->retain();
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
			mLayer->runAction( mFadeInOutAction );
		}
	}
}
