#include "algorithm_practice_floodfill_DirectionMapNodeScene.h"

#include <functional>
#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCTextureCache.h"
#include "ui/UIButton.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "cpg_TileSheetConfiguration.h"
#include "cpg_ui_ToolBarNode.h"

#include "algorithm_practice_floodfill_DirectionMapNode.h"

USING_NS_CC;

namespace
{
	const std::size_t GRID_WIDTH = 3;
	const std::size_t GRID_HEIGHT = 3;
}

namespace algorithm_practice_floodfill
{
	DirectionMapNodeScene::DirectionMapNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mDirectionMapNode( nullptr )
	{}

	Scene* DirectionMapNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) DirectionMapNodeScene( back_to_the_previous_scene_callback );
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

	bool DirectionMapNodeScene::init()
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
			ss << "[1-5] : " << "Show Direction";
			ss << cpg::linefeed;
			ss << "[6] : " << "Clear";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[SPACE] : " << "Fill All";
			ss << cpg::linefeed;
			ss << "[R] : " << "Clean All";

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
		// Direction Maps
		//
		{
			mDirectionMapNode = algorithm_practice_floodfill::DirectionMapNode::create( { GRID_WIDTH, GRID_HEIGHT } );
			mDirectionMapNode->setPosition(
				visibleCenter
				- Vec2( mDirectionMapNode->getContentSize().width * 0.5f, mDirectionMapNode->getContentSize().height * 0.5f )
			);
			addChild( mDirectionMapNode );
		}

		//
		// Setup
		//

		return true;
	}

	void DirectionMapNodeScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( DirectionMapNodeScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void DirectionMapNodeScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void DirectionMapNodeScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		switch( key_code )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_1:
			mDirectionMapNode->UpdateTile( 1, 1, 0 );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			mDirectionMapNode->UpdateTile( 1, 1, cpg::Direction4::eState::Up );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			mDirectionMapNode->UpdateTile( 1, 1, cpg::Direction4::eState::Up | cpg::Direction4::eState::Right );
			break;
		case EventKeyboard::KeyCode::KEY_4:
			mDirectionMapNode->UpdateTile( 1, 1, cpg::Direction4::eState::Up | cpg::Direction4::eState::Right | cpg::Direction4::eState::Down );
			break;
		case EventKeyboard::KeyCode::KEY_5:
			mDirectionMapNode->UpdateTile( 1, 1, cpg::Direction4::eState::Up | cpg::Direction4::eState::Right | cpg::Direction4::eState::Down | cpg::Direction4::eState::Left );
			break;

		case EventKeyboard::KeyCode::KEY_6:
			mDirectionMapNode->ClearTile( 1, 1 );
			break;

		case EventKeyboard::KeyCode::KEY_SPACE:
			for( int ty = 0; GRID_HEIGHT > ty; ++ty )
			{
				for( int tx = 0; GRID_WIDTH > tx; ++tx )
				{
					mDirectionMapNode->UpdateTile( tx, ty, cpg::Direction4::eState::Up | cpg::Direction4::eState::Right | cpg::Direction4::eState::Down | cpg::Direction4::eState::Left );
				}
			}
			break;
		case EventKeyboard::KeyCode::KEY_R:
			for( int ty = 0; GRID_HEIGHT > ty; ++ty )
			{
				for( int tx = 0; GRID_WIDTH > tx; ++tx )
				{
					mDirectionMapNode->ClearTile( tx, ty );
				}
			}
			break;
		}
	}
}
