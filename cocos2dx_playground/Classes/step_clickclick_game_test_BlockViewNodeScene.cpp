#include "step_clickclick_game_test_BlockViewNodeScene.h"

#include <cassert>
#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_clickclick_game_BlockViewNode.h"

#include "step_clickclick_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_TestActionView = 20140416;

	const Size BlockSize( 32.f, 32.f );
	const int BlockCount = 3;
}

namespace step_clickclick
{
	namespace game_test
	{
		BlockViewNodeScene::BlockViewNodeScene() :
			mKeyboardListener( nullptr )
			, mTestActionType( eTestActionType::Increase )
			, mBlockType( game::eBlockType::Single )
			, mBlockLife( 10 )
			, mBlockViewNode( nullptr )
		{}

		Scene* BlockViewNodeScene::create()
		{
			auto ret = new ( std::nothrow ) BlockViewNodeScene();
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

		bool BlockViewNodeScene::init()
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
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Arrow L/R] : Change Block Type";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[1] : Test Action Change : Increase";
				ss << cpg::linefeed;
				ss << "[2] : Test Action Change : Decrease";
				ss << cpg::linefeed;
				ss << "[3] : Test Action Change : Die";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Mouse] : Block Click : Play Test Action";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
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
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Block View
			//
			{
				mBlockViewNode = step_clickclick::game::BlockViewNode::create(
					0
					, BlockSize
					, std::bind( &BlockViewNodeScene::onBlock, this, std::placeholders::_1 )
				);
				mBlockViewNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mBlockViewNode );
			}

			//
			// Setup
			//
			mBlockViewNode->Reset( mBlockType, mBlockLife );
			updateTestAction( mTestActionType );

			return true;
		}

		void BlockViewNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BlockViewNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BlockViewNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void BlockViewNodeScene::onBlock( const int block_index )
		{
			CCLOG( "onBlock : %d", block_index );

			switch( mTestActionType )
			{
			case eTestActionType::Increase:
				++mBlockLife;
				break;
			case eTestActionType::Decrease:
				--mBlockLife;
				break;
			case eTestActionType::Die:
				mBlockLife = 0;
				break;

			default:
				assert( false );
			}

			mBlockViewNode->UpdateLife( mBlockLife );
		}


		void BlockViewNodeScene::updateTestAction( const eTestActionType test_action_type )
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


		void BlockViewNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_clickclick::RootScene::create() );
				break;

			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			{
				const auto current_block_type = static_cast<int>( mBlockType );
				auto new_block_type = std::max( static_cast<int>( game::eBlockType::FIRST ), current_block_type - 1 );
				if( current_block_type != new_block_type )
				{
					mBlockType = static_cast<game::eBlockType>( new_block_type );
					mBlockViewNode->Reset( mBlockType, mBlockLife );
				}
			}
			break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			{
				const auto current_block_type = static_cast<int>( mBlockType );
				auto new_block_type = std::min( static_cast<int>( game::eBlockType::END ), current_block_type + 1 );
				if( current_block_type != new_block_type )
				{
					mBlockType = static_cast<game::eBlockType>( new_block_type );
					mBlockViewNode->Reset( mBlockType, mBlockLife );
				}
			}
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
				mBlockLife = 10;
				mBlockViewNode->Reset( mBlockType, mBlockLife );
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
