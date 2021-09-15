#include "ui_research_gauge_research_AnimationScene.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCDrawNode.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const Color4F GaugeColor1( Color4F::GREEN );
	const Color4F GaugeColor2( Color4F::ORANGE );

	const Size GaugeSize( 100.f, 20.f );

	const int GaugeAmountOfTotal = 85;
	const int GaugeAmountOfChange = 20;
}

namespace ui_research
{
	namespace gauge_research
	{
		AnimationScene::AnimationScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )

			, mGaugeViewNode( nullptr )
			, mGaugeMax( GaugeAmountOfTotal )
			, mGaugeMin( 0 )
			, mGaugeCurrent( GaugeAmountOfTotal )
			, mGaugeAnimationCurrent( GaugeAmountOfTotal )

			, mGaugeAnimationViewNode( nullptr )

			, mGaugeStatisticsViewNode( nullptr )
			, mGaugeAnimationStatisticsViewNode( nullptr )
		{}

		Scene* AnimationScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) AnimationScene( back_to_the_previous_scene_callback );
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

		bool AnimationScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();
			const Vec2 visibleCenter(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			);

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << "[1] : Decrease " << GaugeAmountOfChange << cpg::linefeed;
				ss << "[2] : Increase " << GaugeAmountOfChange << cpg::linefeed;

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( 0.f, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Test Gauge
			//
			{
				mGaugeViewNode = DrawNode::create();
				mGaugeViewNode->setPosition(
					Vec2( visibleCenter.x, visibleSize.height * 0.4f )
					- Vec2( GaugeSize.width * 0.5f, GaugeSize.height * 0.5f )
				);
				addChild( mGaugeViewNode, 1 );

				mGaugeAnimationViewNode = DrawNode::create();
				mGaugeAnimationViewNode->setPosition(
					Vec2( visibleCenter.x, visibleSize.height * 0.43f )
					- Vec2( GaugeSize.width * 0.5f, GaugeSize.height * 0.5f )
				);
				addChild( mGaugeAnimationViewNode, 0 );
			}

			//
			// Test Gauge Stat View
			//
			{
				mGaugeStatisticsViewNode = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				mGaugeStatisticsViewNode->setColor( Color3B( GaugeColor1 ) );
				mGaugeStatisticsViewNode->setPosition(
					Vec2( visibleCenter.x, visibleSize.height * 0.6f )
				);
				addChild( mGaugeStatisticsViewNode );

				mGaugeAnimationStatisticsViewNode = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				mGaugeAnimationStatisticsViewNode->setColor( Color3B( GaugeColor2 ) );
				mGaugeAnimationStatisticsViewNode->setPosition(
					Vec2( visibleCenter.x, visibleSize.height * 0.63f )
				);
				addChild( mGaugeAnimationStatisticsViewNode );
			}

			//
			//
			//
			updateGaugeView();
			updateGaugeAnimationView();

			return true;
		}

		void AnimationScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( AnimationScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void AnimationScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void AnimationScene::updateGaugeView()
		{
			const float gauge_rate = static_cast<float>( mGaugeCurrent ) / static_cast<float>( mGaugeMax );

			mGaugeViewNode->clear();
			mGaugeViewNode->drawSolidRect(
				Vec2::ZERO
				, Vec2( GaugeSize.width * gauge_rate, GaugeSize.height )
				, GaugeColor1
			);

			mGaugeStatisticsViewNode->setString( StringUtils::format(
				"%d / %d"
				, mGaugeCurrent
				, mGaugeMax
			) );
		}
		void AnimationScene::updateGaugeAnimationView()
		{
			const float gauge_rate = static_cast<float>( mGaugeAnimationCurrent ) / static_cast<float>( mGaugeMax );

			mGaugeAnimationViewNode->clear();
			mGaugeAnimationViewNode->drawSolidRect(
				Vec2::ZERO
				, Vec2( GaugeSize.width * gauge_rate, GaugeSize.height )
				, GaugeColor2
			);

			mGaugeAnimationStatisticsViewNode->setString( StringUtils::format(
				"%d / %d"
				, mGaugeAnimationCurrent
				, mGaugeMax
			) );
		}


		void AnimationScene::requestUpdateGaugeAnimation()
		{
			if( !isScheduled( schedule_selector( AnimationScene::update4GaugeAnimation ) ) )
			{
				schedule( schedule_selector( AnimationScene::update4GaugeAnimation ) );
			}
		}
		void AnimationScene::update4GaugeAnimation( float delta_time )
		{
			if( mGaugeCurrent == mGaugeAnimationCurrent )
			{
				unschedule( schedule_selector( AnimationScene::update4GaugeAnimation ) );
				return;
			}

			if( mGaugeCurrent > mGaugeAnimationCurrent )
			{
				mGaugeAnimationCurrent = std::min( mGaugeCurrent, mGaugeAnimationCurrent + 1 );
			}
			else //if( mGaugeCurrent < mGaugeAnimationCurrent )
			{
				mGaugeAnimationCurrent = std::max( mGaugeCurrent, mGaugeAnimationCurrent - 1 );
			}

			updateGaugeAnimationView();
		}


		void AnimationScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*key_event*/ )
		{
			switch( key_code )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_1:
				mGaugeCurrent = std::max( mGaugeMin, mGaugeCurrent - GaugeAmountOfChange );
				updateGaugeView();
				requestUpdateGaugeAnimation();
				return;
			case EventKeyboard::KeyCode::KEY_2:
				mGaugeCurrent = std::min( mGaugeMax, mGaugeCurrent + GaugeAmountOfChange );
				updateGaugeView();
				requestUpdateGaugeAnimation();
				return;
			}
		}
	}
}
