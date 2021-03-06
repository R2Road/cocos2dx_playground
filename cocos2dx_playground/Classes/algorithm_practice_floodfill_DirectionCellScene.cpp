#include "algorithm_practice_floodfill_DirectionCellScene.h"

#include <functional>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_StringTable.h"

USING_NS_CC;

namespace algorithm_practice_floodfill
{
	DirectionCellScene::DirectionCellScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mDirectionCell()
		, mDirectionView( nullptr )
	{}

	Scene* DirectionCellScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) DirectionCellScene( back_to_the_previous_scene_callback );
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

	bool DirectionCellScene::init()
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
			ss << std::endl;
			ss << std::endl;
			ss << "[ESC] : Return to Root";
			ss << std::endl;
			ss << std::endl;
			ss << "[Arrow R] : " << "Rotate Right";
			ss << std::endl;
			ss << "[Arrow L] : " << "Rotate Left";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 7, Size::ZERO, TextHAlignment::LEFT );
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
		// Setup
		//
		updateDirectionView();

		return true;
	}

	void DirectionCellScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( DirectionCellScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void DirectionCellScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void DirectionCellScene::updateDirectionView()
	{
		switch( mDirectionCell.GetCurrentDirection().GetState() )
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
	}


	void DirectionCellScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		switch( key_code )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			mDirectionCell.RotateCurrentDirection( false );
			updateDirectionView();
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			mDirectionCell.RotateCurrentDirection( true );
			updateDirectionView();
			break;
		}
	}
}
