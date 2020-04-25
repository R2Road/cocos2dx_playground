#include "step_clickclick_game_BlockTestScene.h"

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
#include "base/ccUTF8.h"

#include "step_clickclick_RootScene.h"

#include "step_clickclick_game_BlockView.h"

USING_NS_CC;

namespace
{
	const int TAG_TestActionView = 20140416;
	const int TAG_SelectedBlockTypeView = 20160528;

	const Size BlockSize( 32.f, 32.f );
}

namespace step_clickclick
{
	namespace game
	{
		BlockTestScene::BlockTestScene() :
			mKeyboardListener( nullptr )
			, mTestActionType( eTestActionType::Increase )
		{}

		Scene* BlockTestScene::create()
		{
			auto ret = new ( std::nothrow ) BlockTestScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool BlockTestScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

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
				ss << "[F1] : Reset";
				ss << std::endl;
				ss << std::endl;
				ss << "[1] : Test Action Change : Increase";
				ss << std::endl;
				ss << "[2] : Test Action Change : Decrease";
				ss << std::endl;
				ss << "[3] : Test Action Change : Die";
				ss << std::endl;
				ss << std::endl;
				ss << "[Mouse] : Click : Play Test Action";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 10 );
				label->setTag( TAG_TestActionView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateTestAction( mTestActionType );
			}

			//
			// Selected Block Type View
			//
			{
				auto label = Label::createWithTTF( "Block Type : -", "fonts/arial.ttf", 10 );
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
				int block_type = static_cast<int>( eBlockType::Single );
				for( int i = 0; 3 > i; ++i, ++block_type )
				{
					mBlockContainer.emplace_back( i );
					mBlockContainer[i].Reset( static_cast<eBlockType>( block_type ), 10 );
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

				BlockView* block_view = nullptr;
				int i = 0;
				for( const auto& b : mBlockContainer )
				{
					block_view = BlockView::create( b.GetIndex(), BlockSize, std::bind( &BlockTestScene::onGameProcess, this, std::placeholders::_1 ) );
					block_view->setTag( b.GetIndex() );
					block_view->Reset( b.GetType(), b.GetLife() );
					block_view->setPosition(
						BlockStartPosition
						+ ( BlockSpacing * i )
					);
					addChild( block_view );

					++i;
				}
			}

			return true;
		}

		void BlockTestScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BlockTestScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BlockTestScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void BlockTestScene::onGameProcess( const int block_linear_index )
		{
			experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );

			auto& target_block_data = mBlockContainer[block_linear_index];
			updateSelectedBlockTypeView( target_block_data.GetType() );

			const int last_life = target_block_data.GetLife();
			switch( mTestActionType )
			{
			case eTestActionType::Increase:
				target_block_data.IncreaseAction();
				break;
			case eTestActionType::Decrease:
				target_block_data.DecreaseAction();
				break;
			case eTestActionType::Die:
				target_block_data.DieAction();
				break;
			default:
				assert( false );
			}

			auto block_view = static_cast<BlockView*>( getChildByTag( block_linear_index ) );
			block_view->UpdateLife( last_life, target_block_data.GetLife() );
		}


		void BlockTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_clickclick::RootScene::create() );
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

			case EventKeyboard::KeyCode::KEY_F1:
			{
				for( auto& b : mBlockContainer )
				{
					b.Reset( b.GetType(), 10 );

					auto block_view = static_cast<BlockView*>( getChildByTag( b.GetIndex() ) );
					block_view->Reset( b.GetType(), b.GetLife() );
					block_view->SetVisible( true );
				}
			}
			break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void BlockTestScene::updateTestAction( const eTestActionType test_action_type )
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
		void BlockTestScene::updateSelectedBlockTypeView( const eBlockType block_type )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_SelectedBlockTypeView ) );
			switch( block_type )
			{
			case eBlockType::Single:
				label->setString( "Block Type : Single" );
				break;
			case eBlockType::Same:
				label->setString( "Block Type : Same" );
				break;
			case eBlockType::Different:
				label->setString( "Block Type : Different" );
				break;
			default:
				assert( false );
			}
		}
	} // namespace game
} // namespace step_clickclick
