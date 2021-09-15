#include "ui_research_gauge_research_SpriteScene.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const Color4F GaugeColor( Color4F::GREEN );
	const Size GaugeSize( 100.f, 20.f );

	const int GaugeAmountOfTotal = 85;
	const int GaugeAmountOfChange = 10;
}

namespace ui_research
{
	namespace gauge_research
	{
		SpriteScene::SpriteScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )

			, mGaugeViewNode1( nullptr )
			, mGaugeMax( GaugeAmountOfTotal )
			, mGaugeMin( 0 )
			, mGaugeCurrent( GaugeAmountOfTotal )

			, mGaugeStatisticsViewNode( nullptr )
		{}

		Scene* SpriteScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) SpriteScene( back_to_the_previous_scene_callback );
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

		bool SpriteScene::init()
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
				mGaugeViewNode1 = Sprite::createWithSpriteFrameName( "s9_normal_3.png" );
				mGaugeViewNode1->setAnchorPoint( Vec2::ZERO );
				mGaugeViewNode1->setContentSize( GaugeSize );
				mGaugeViewNode1->setPosition(
					Vec2( visibleCenter.x, visibleSize.height * 0.4f )
					- Vec2( GaugeSize.width * 0.5f, GaugeSize.height * 0.5f )
				);
				addChild( mGaugeViewNode1 );
			}

			//
			// Test Gauge Statistics View
			//
			{
				mGaugeStatisticsViewNode = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				mGaugeStatisticsViewNode->setPosition(
					Vec2( visibleCenter.x, visibleSize.height * 0.6f )
				);
				addChild( mGaugeStatisticsViewNode );
			}

			//
			//
			//
			updateGaugeView();
			updateGaugeStatisticsView();

			return true;
		}

		void SpriteScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SpriteScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void SpriteScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void SpriteScene::updateGaugeStatisticsView()
		{
			mGaugeStatisticsViewNode->setString( StringUtils::format(
				"%d / %d"
				, mGaugeCurrent
				, mGaugeMax
			) );
		}
		void SpriteScene::updateGaugeView()
		{
			const float gauge_rate = static_cast<float>( mGaugeCurrent ) / static_cast<float>( mGaugeMax );

			mGaugeViewNode1->setContentSize( Size( GaugeSize.width * gauge_rate, GaugeSize.height ) );
		}


		void SpriteScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*key_event*/ )
		{
			switch( key_code )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_1:
				mGaugeCurrent = std::max( mGaugeMin, mGaugeCurrent - GaugeAmountOfChange );
				updateGaugeView();
				updateGaugeStatisticsView();
				return;
			case EventKeyboard::KeyCode::KEY_2:
				mGaugeCurrent = std::min( mGaugeMax, mGaugeCurrent + GaugeAmountOfChange );
				updateGaugeView();
				updateGaugeStatisticsView();
				return;
			}
		}
	}
}
