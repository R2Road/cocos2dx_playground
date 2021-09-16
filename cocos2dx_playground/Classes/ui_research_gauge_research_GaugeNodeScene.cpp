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
	const Color4F GaugeColor1( Color4F::GREEN );
	const Color4F GaugeColor2( Color4F::ORANGE );

	const Size GaugeSize( 100.f, 20.f );

	const int GaugeAmountOfChange = 20;
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
				mGaugeNode = cpg_ui::GaugeNode::create( GaugeSize, 85 );
				mGaugeNode->setPosition(
					Vec2( visibleCenter.x, visibleSize.height * 0.6f )
				);
				addChild( mGaugeNode );
			}

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


		void GaugeNodeScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*key_event*/ )
		{
			switch( key_code )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_1:
				mGaugeCurrent = std::max( 0, mGaugeCurrent - GaugeAmountOfChange );
				mGaugeNode->UpdateCurrent( mGaugeCurrent );
				return;
			case EventKeyboard::KeyCode::KEY_2:
				mGaugeCurrent = std::min( mGaugeMax, mGaugeCurrent + GaugeAmountOfChange );
				mGaugeNode->UpdateCurrent( mGaugeCurrent );
				return;
			}
		}
	}
}
