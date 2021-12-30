#include "algorithm_practice_astar_Direction8Scene.h"

#include <new>
#include <numeric>

#include "2d/CCDrawNode.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace algorithm_practice_astar
{
	Direction8Scene::Direction8Scene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mDirection8()
		, mDirectionView( nullptr )
		, mDirectionIndicator( nullptr )
	{}

	Scene* Direction8Scene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) Direction8Scene( back_to_the_previous_scene_callback );
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

	bool Direction8Scene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleOrigin = _director->getVisibleOrigin();
		const auto visibleSize = _director->getVisibleSize();
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
			ss << cpg::linefeed;
			ss << "[Arrow R] : " << "Rotate Right";
			ss << cpg::linefeed;
			ss << "[Arrow L] : " << "Rotate Left";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 7 );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition(
				visibleOrigin
				+ Vec2( 0.f, visibleSize.height )
			);
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Background
		//
		{
			auto layer = LayerColor::create( Color4B( 8, 45, 48, 255 ) );
			addChild( layer, std::numeric_limits<int>::min() );
		}

		//
		// Direction View
		//
		{
			mDirectionView = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 14 );
			mDirectionView->setPosition( visibleCenter );
			addChild( mDirectionView );
		}

		//
		// Indicator
		//
		{
			mDirectionIndicator = DrawNode::create();
			mDirectionIndicator->setPosition( visibleCenter );
			addChild( mDirectionIndicator );
		}

		//
		// Setup
		//
		updateDirectionView();

		return true;
	}

	void Direction8Scene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( Direction8Scene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void Direction8Scene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void Direction8Scene::updateDirectionView()
	{
		switch( mDirection8.GetState() )
		{
		case cpg::Direction8::eState::Up:
			mDirectionView->setString( "Up" );
			break;
		case cpg::Direction8::eState::UpRight:
			mDirectionView->setString( "Up Right" );
			break;
		case cpg::Direction8::eState::Right:
			mDirectionView->setString( "Right" );
			break;
		case cpg::Direction8::eState::DownRight:
			mDirectionView->setString( "Down Right" );
			break;
		case cpg::Direction8::eState::Down:
			mDirectionView->setString( "Down" );
			break;
		case cpg::Direction8::eState::DownLeft:
			mDirectionView->setString( "Down Left" );
			break;
		case cpg::Direction8::eState::Left:
			mDirectionView->setString( "Left" );
			break;
		case cpg::Direction8::eState::UpLeft:
			mDirectionView->setString( "Up Left" );
			break;

		default:
			mDirectionView->setString( "WTF" );
			break;
		}

		mDirectionIndicator->clear();
		mDirectionIndicator->drawDot( Vec2( mDirection8.GetPoint().x * 40.f, mDirection8.GetPoint().y * 40.f ), 3.f, Color4F::GREEN );
	}


	void Direction8Scene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		switch( key_code )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			mDirection8.Rotate( false );
			updateDirectionView();
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			mDirection8.Rotate( true );
			updateDirectionView();
			break;
		}
	}
}
