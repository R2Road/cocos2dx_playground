#include "algorithm_practice_floodfill_Direction4Scene.h"

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

namespace algorithm_practice_floodfill
{
	Direction4Scene::Direction4Scene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mDirection4()
		, mDirectionView( nullptr )
		, mDirectionIndicator( nullptr )
	{}

	Scene* Direction4Scene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) Direction4Scene( back_to_the_previous_scene_callback );
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

	bool Direction4Scene::init()
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

	void Direction4Scene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( Direction4Scene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void Direction4Scene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void Direction4Scene::updateDirectionView()
	{
		switch( mDirection4.GetState() )
		{
		case cpg::Direction4::eState::Up:
			mDirectionView->setString( "Up" );
			break;
		case cpg::Direction4::eState::Right:
			mDirectionView->setString( "Right" );
			break;
		case cpg::Direction4::eState::Down:
			mDirectionView->setString( "Down" );
			break;
		case cpg::Direction4::eState::Left:
			mDirectionView->setString( "Left" );
			break;

		default:
			mDirectionView->setString( "WTF" );
			break;
		}

		mDirectionIndicator->clear();
		mDirectionIndicator->drawDot( Vec2( mDirection4.GetPoint().x * 40.f, mDirection4.GetPoint().y * 40.f ), 3.f, Color4F::GREEN );
	}


	void Direction4Scene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		switch( key_code )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			mDirection4.Rotate( false );
			updateDirectionView();
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			mDirection4.Rotate( true );
			updateDirectionView();
			break;
		}
	}
}
