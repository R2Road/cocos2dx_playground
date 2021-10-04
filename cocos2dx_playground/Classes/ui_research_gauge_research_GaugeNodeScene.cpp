#include "ui_research_gauge_research_GaugeNodeScene.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/ccUTF8.h"

#include "cpg_ui_GaugeNode.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const Size GaugeSize( 100.f, 20.f );

	const int GaugeAmountOfChange = 20;
	const int GaugeMaxAmountOfChange = 10;
}

namespace ui_research
{
	namespace gauge_research
	{
		GaugeNodeScene::GaugeNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )

			, mGaugeMax( 85 )
			, mGaugeCurrent( 85 )

			, mGaugeNode( nullptr )
			, mStatisticsViewNode( nullptr )
		{}

		Scene* GaugeNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) GaugeNodeScene( back_to_the_previous_scene_callback );
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

		bool GaugeNodeScene::init()
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
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed << cpg::linefeed;
				ss << "[1/2] : Decrease/Increase Current " << GaugeAmountOfChange << cpg::linefeed;
				ss << "[Q/W] : Decrease/Increase Max " << GaugeAmountOfChange << cpg::linefeed;

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
				mGaugeNode = cpg_ui::GaugeNode::create( GaugeSize );
				mGaugeNode->setPosition(
					Vec2( visibleCenter.x, visibleSize.height * 0.4f )
				);
				addChild( mGaugeNode );
			}

			//
			// Statistics View
			//
			{
				mStatisticsViewNode = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::LEFT );
				mStatisticsViewNode->setPosition(
					Vec2( visibleCenter.x, visibleSize.height * 0.7f )
				);
				addChild( mStatisticsViewNode );
			}

			//
			//
			//
			updateGauge();

			return true;
		}

		void GaugeNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GaugeNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void GaugeNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void GaugeNodeScene::updateGauge()
		{
			const float gauge_rate = static_cast<float>( mGaugeCurrent ) / static_cast<float>( mGaugeMax );
			mGaugeNode->UpdateCurrent( gauge_rate );

			mStatisticsViewNode->setString( StringUtils::format(
				"%d / %d"
				, mGaugeCurrent
				, mGaugeMax
			) );
		}
		void GaugeNodeScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*key_event*/ )
		{
			switch( key_code )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_1:
				mGaugeCurrent = std::max( 0, mGaugeCurrent - GaugeAmountOfChange );
				updateGauge();
				return;
			case EventKeyboard::KeyCode::KEY_2:
				mGaugeCurrent = std::min( mGaugeMax, mGaugeCurrent + GaugeAmountOfChange );
				updateGauge();
				return;

			case EventKeyboard::KeyCode::KEY_Q:
				mGaugeMax = std::max( 0, mGaugeMax - GaugeMaxAmountOfChange );
				mGaugeCurrent = std::min( mGaugeCurrent, mGaugeMax );
				updateGauge();
				return;
			case EventKeyboard::KeyCode::KEY_W:
				mGaugeMax = mGaugeMax + GaugeMaxAmountOfChange;
				updateGauge();
				return;
			}
		}
	}
}
