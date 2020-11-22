#include "step_clickclick_game_test_BlockScene.h"

#include <cassert>
#include <functional>
#include <new>
#include <numeric>
#include <sstream>

#include "audio/include/AudioEngine.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_StringTable.h"

#include "step_clickclick_RootScene.h"

#include "step_clickclick_game_BlockView.h"

USING_NS_CC;

namespace
{
	const int TAG_TestActionView = 20140416;
	const int TAG_SelectedBlockTypeView = 20160528;

	const Size BlockSize( 32.f, 32.f );
	const int BlockCount = 3;
}

namespace step_clickclick
{
	namespace game_test
	{
		BlockScene::BlockScene() :
			mKeyboardListener( nullptr )
			, mTestActionType( eTestActionType::Increase )
			, mBlockContainer()
			, mBlockViewContainer()
		{}

		Scene* BlockScene::create()
		{
			auto ret = new ( std::nothrow ) BlockScene();
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

		bool BlockScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();

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
				ss << "[1] : Test Action Change : Increase";
				ss << std::endl;
				ss << "[2] : Test Action Change : Decrease";
				ss << std::endl;
				ss << "[3] : Test Action Change : Die";
				ss << std::endl;
				ss << std::endl;
				ss << "[Mouse] : Block Click : Play Test Action";
				ss << std::endl;
				ss << std::endl;
				ss << "[R] : Reset";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 0, 41, 13, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Test Action
			//
			{
				auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12 );
				label->setTag( TAG_TestActionView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateTestAction( mTestActionType );
			}

			//
			// Selected Block Type View
			//
			{
				auto label = Label::createWithTTF( "Block Type : -", cpg::StringTable::GetFontPath(), 12 );
				label->setTag( TAG_SelectedBlockTypeView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Block
			//
			{
				int block_type = static_cast<int>( step_clickclick::game::eBlockType::Single );
				for( int i = 0; BlockCount > i; ++i, ++block_type )
				{
					mBlockContainer.emplace_back( i );
					mBlockContainer[i].Reset( static_cast<step_clickclick::game::eBlockType>( block_type ), 10 );
				}
			}

			//
			// Block View
			//
			{
				const Vec2 BlockStartPosition(
					visibleOrigin.x + visibleSize.width * 0.3f
					, visibleOrigin.y + visibleSize.height * 0.5f
				);
				const Vec2 BlockSpacing(
					visibleSize.width * 0.2f
					, 0.f
				);

				step_clickclick::game::BlockView* block_view = nullptr;
				for( int i = 0; BlockCount > i; ++i )
				{
					block_view = step_clickclick::game::BlockView::create( i, BlockSize, std::bind( &BlockScene::onGameProcess, this, std::placeholders::_1 ) );
					block_view->setTag( i );
					block_view->setPosition(
						BlockStartPosition
						+ ( BlockSpacing * i )
					);
					addChild( block_view );

					mBlockViewContainer.emplace_back( block_view );
				}
			}

			//
			// Setup
			//
			ResetBlockContainer();

			return true;
		}

		void BlockScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BlockScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BlockScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void BlockScene::onGameProcess( const int block_linear_index )
		{
			experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );

			auto& target_block_data = mBlockContainer[block_linear_index];
			updateSelectedBlockTypeView( target_block_data.GetType() );

			const int last_life = target_block_data.GetLife();
			switch( mTestActionType )
			{
			case eTestActionType::Increase:
				target_block_data.IncreaseLife();
				break;
			case eTestActionType::Decrease:
				target_block_data.DecreaseLife();
				break;
			case eTestActionType::Die:
				target_block_data.Die();
				break;
			default:
				assert( false );
			}

			auto block_view = static_cast<step_clickclick::game::BlockView*>( getChildByTag( block_linear_index ) );
			block_view->UpdateLife( last_life, target_block_data.GetLife() );
		}


		void BlockScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_clickclick::RootScene::create() );
				break;

			case EventKeyboard::KeyCode::KEY_1:
				updateTestAction( eTestActionType::Increase );
				break;
			case EventKeyboard::KeyCode::KEY_2:
				updateTestAction( eTestActionType::Decrease );
				break;
			case EventKeyboard::KeyCode::KEY_3:
				updateTestAction( eTestActionType::Die );
				break;

			case EventKeyboard::KeyCode::KEY_R:
				ResetBlockContainer();
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void BlockScene::updateTestAction( const eTestActionType test_action_type )
		{
			mTestActionType = test_action_type;

			auto label = static_cast<Label*>( getChildByTag( TAG_TestActionView ) );
			switch( mTestActionType )
			{
			case eTestActionType::Increase:
				label->setString( "Current Test Action : Increase" );
				break;
			case eTestActionType::Decrease:
				label->setString( "Current Test Action : Decrease" );
				break;
			case eTestActionType::Die:
				label->setString( "Current Test Action : Die" );
				break;
			default:
				assert( false );
			}
		}
		void BlockScene::updateSelectedBlockTypeView( const step_clickclick::game::eBlockType block_type )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_SelectedBlockTypeView ) );
			switch( block_type )
			{
			case step_clickclick::game::eBlockType::Single:
				label->setString( "Block Type : Single" );
				break;
			case step_clickclick::game::eBlockType::Same:
				label->setString( "Block Type : Same" );
				break;
			case step_clickclick::game::eBlockType::Different:
				label->setString( "Block Type : Different" );
				break;
			default:
				assert( false );
			}
		}

		void BlockScene::ResetBlockContainer()
		{
			for( int i = 0; BlockCount > i; ++i )
			{
				auto& target_block = mBlockContainer[i];
				target_block.Reset( target_block.GetType(), 10 );

				auto block_view = mBlockViewContainer[i];
				block_view->Reset( target_block.GetType(), target_block.GetLife() );
				block_view->SetVisible( true );
			}
		}
	} // namespace game
} // namespace step_clickclick
