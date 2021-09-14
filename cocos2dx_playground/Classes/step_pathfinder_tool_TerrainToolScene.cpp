#include "step_pathfinder_tool_TerrainToolScene.h"

#include <new>
#include <numeric>

#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "platform/CCFileUtils.h"
#include "ui/UIButton.h"
#include "ui/UITextField.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "helper_Win32DirectoryOpen.h"

#include "step_pathfinder_game_TerrainViewer.h"
#include "step_pathfinder_tool_TerrainSaveHelper.h"
#include "step_pathfinder_tool_TerrainEditHelper.h"
#include "step_pathfinder_tool_ui_TileSelectNode.h"

USING_NS_CC;

namespace
{
	const int TAG_TextField = 20140416;
}

namespace step_pathfinder
{
	namespace tool
	{
		TerrainToolScene::TerrainToolScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mTerrainData()
			, mCurrentTileType( step_pathfinder::game::eTileType::road )

			, mTerrainViewer( nullptr )
		{}

		Scene* TerrainToolScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) TerrainToolScene( back_to_the_previous_scene_callback );
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

		bool TerrainToolScene::init()
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

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( 0.f, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Target Path
			//
			{
				const float MAX_LINE_WIDTH = visibleSize.width * 0.8f;

				// Title
				{
					auto label = Label::createWithTTF( "<Target Path>", cpg::StringTable::GetFontPath(), 14 );
					label->setAnchorPoint( Vec2( 1.f, 0.f ) );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width - 4.f, visibleSize.height * 0.93f )
					);
					addChild( label );

					// Open Folder
					if( helper::isEnableWin32DirectoryOpen() )
					{
						auto button = ui::Button::create( "guide_01_2.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
						button->setScale9Enabled( true );
						button->addTouchEventListener( []( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type ) {
							if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
							{
								return;
							}

							helper::Win32DirectoryOpen( FileUtils::getInstance()->getWritablePath().c_str() );
						} );
						addChild( button );
						{
							auto title_label = Label::createWithTTF( "Open Folder", cpg::StringTable::GetFontPath(), 10 );
							button->setTitleLabel( title_label );

							button->setContentSize( title_label->getContentSize() + Size( 10.f, 4.f ) + Size( 10.f, 4.f ) );
						}

						button->setPosition(
							label->getPosition()
							+ Vec2(
								-label->getContentSize().width - 4.f - ( button->getContentSize().width * 0.5f )
								, button->getContentSize().height * 0.5f
							)
						);
					}
				}

				// Path
				{
					auto label = Label::createWithTTF( FileUtils::getInstance()->getWritablePath().c_str(), cpg::StringTable::GetFontPath(), 10 );
					label->setAnchorPoint( Vec2( 1.f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setMaxLineWidth( MAX_LINE_WIDTH );
					label->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width - 4.f, visibleSize.height * 0.93f )
					);
					addChild( label );
				}
			}

			//
			// Terrain Viewer
			//
			{
				mTerrainViewer = game::TerrainViewer::create( mTerrainData.GetWidth(), mTerrainData.GetHeight(), Size( 32, 32 ) );
				mTerrainViewer->setPosition(
					visibleOrigin
					+ Vec2( ( visibleSize.width - mTerrainViewer->getContentSize().width ) * 0.5f, ( visibleSize.height - mTerrainViewer->getContentSize().height ) * 0.6f )
				);
				addChild( mTerrainViewer );

				// apply terrain data
				mTerrainViewer->LoadTerrainData4Original( mTerrainData );
			}

			//
			// Terrain Editor
			//
			{
				auto terrain_edit_helper = TerrainEditHelper::create( mTerrainData.GetWidth(), mTerrainData.GetHeight(), Size( 32, 32 ), CC_CALLBACK_2( TerrainToolScene::onGrid, this ) );
				terrain_edit_helper->setPosition(
					visibleOrigin
					+ Vec2( ( visibleSize.width - terrain_edit_helper->getContentSize().width ) * 0.5f, ( visibleSize.height - terrain_edit_helper->getContentSize().height ) * 0.6f )
				);
				addChild( terrain_edit_helper, 1 );
			}

			//
			// ui - tile select
			//
			{
				auto tile_select_node = tool_ui::TileSelectNode::create( std::bind( &TerrainToolScene::onTileSelect, this, std::placeholders::_1 ) );
				tile_select_node->setPositionY(
					visibleOrigin.y
					+ ( visibleSize.height * 0.5f )
					- ( tile_select_node->getContentSize().height * 0.5f )
				);
				addChild( tile_select_node );

				//
				// indicator setup
				//
				tile_select_node->SetIndicator( mCurrentTileType );
			}


			const Size BUTTON_MARGIN( 20.f, 10.f );

			//
			// ui - file name input
			//
			{
				const int TEXT_FIELD_MAX_LENGTH = 20;
				const char TEXT_FIELD_CURSOR_CHAR = 95; // "_"
				const std::string DUMMY_STRING( TEXT_FIELD_MAX_LENGTH, 'A' );
				const std::string PLACE_HOLDER_STRING( "input file name here~!" );

				auto ui_text_field = ui::TextField::create( DUMMY_STRING, cpg::StringTable::GetFontPath(), 12 );
				ui_text_field->setTag( TAG_TextField );
				ui_text_field->setPlaceHolderColor( Color3B::GREEN );
				ui_text_field->setMaxLength( TEXT_FIELD_MAX_LENGTH );
				ui_text_field->setMaxLengthEnabled( true );
				ui_text_field->setCursorChar( TEXT_FIELD_CURSOR_CHAR );
				ui_text_field->setCursorEnabled( true );
				ui_text_field->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.2f )
				);
				addChild( ui_text_field, 0 );

				auto guide_button = ui::Button::create( "guide_01_2.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				guide_button->setScale9Enabled( true );
				guide_button->setContentSize( ui_text_field->getContentSize() + BUTTON_MARGIN + BUTTON_MARGIN );
				guide_button->addTouchEventListener( [ui_text_field]( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type )
				{
					switch( touch_event_type )
					{
					case cocos2d::ui::Widget::TouchEventType::CANCELED:
					case cocos2d::ui::Widget::TouchEventType::ENDED:
					{
						ui_text_field->attachWithIME();
					}
					break;
					}
				} );
				guide_button->setPosition( ui_text_field->getPosition() );
				addChild( guide_button, 1 );

				ui_text_field->setPlaceHolder( PLACE_HOLDER_STRING );
			}

			//
			// ui - file save
			//
			{
				auto button = ui::Button::create( "guide_01_2.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->addTouchEventListener( CC_CALLBACK_2( TerrainToolScene::onSave, this ) );
				addChild( button );
				{
					auto label = Label::createWithTTF( "Save", cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::LEFT );
					label->setColor( Color3B::RED );
					button->setTitleLabel( label );

					button->setContentSize( label->getContentSize() + BUTTON_MARGIN + BUTTON_MARGIN );
				}

				button->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.4f, visibleSize.height * 0.08f )
				);
			}

			//
			// ui - file load
			//
			{
				auto button = ui::Button::create( "guide_01_2.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->addTouchEventListener( CC_CALLBACK_2( TerrainToolScene::onLoad, this ) );
				addChild( button );
				{
					auto label = Label::createWithTTF( "Load", cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::LEFT );
					label->setColor( Color3B::MAGENTA );
					button->setTitleLabel( label );

					button->setContentSize( label->getContentSize() + BUTTON_MARGIN + BUTTON_MARGIN );
				}

				button->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.6f, visibleSize.height * 0.08f )
				);
			}

			return true;
		}

		void TerrainToolScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TerrainToolScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void TerrainToolScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		Node* TerrainToolScene::makeMenuButton( const cocos2d::Size menu_size, const step_pathfinder::game::eTileType tile_type, const char* button_text, const ui::Widget::ccWidgetTouchCallback& callback )
		{
			auto button = ui::Button::create( "guide_01_2.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
			button->setTag( static_cast<int>( tile_type ) );
			button->addTouchEventListener( callback );
			button->setScale9Enabled( true );
			button->setContentSize( menu_size );
			{
				auto label = Label::createWithTTF( button_text, cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
				button->setTitleLabel( label );
			}

			return button;
		}


		void TerrainToolScene::onTileSelect( const step_pathfinder::game::eTileType new_tile_type )
		{
			mCurrentTileType = new_tile_type;
		}


		void TerrainToolScene::onGrid( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			auto button = static_cast<Node*>( sender );
			const int gy = button->getTag() / mTerrainData.GetHeight();
			const int gx = button->getTag() - ( gy * mTerrainData.GetWidth() );
			if( mCurrentTileType == mTerrainData.Get( gx, gy ) )
			{
				return;
			}

			if( step_pathfinder::game::TileType2UniqueFlag( mCurrentTileType ) )
			{
				const auto default_tile_type = step_pathfinder::game::eTileType::road;
				for( int ty = 0; ty < mTerrainData.GetHeight(); ++ty )
				{
					for( int tx = 0; tx < mTerrainData.GetWidth(); ++tx )
					{
						if( mTerrainData.Get( tx, ty ) != mCurrentTileType )
						{
							continue;
						}
						
						mTerrainData.Set( tx, ty, default_tile_type );
						mTerrainViewer->UpdateTile( tx, ty, default_tile_type );
					}
				}
			}

			
			mTerrainData.Set( gx, gy, mCurrentTileType );
			mTerrainViewer->UpdateTile( gx, gy, mCurrentTileType );
		}


		void TerrainToolScene::onSave( cocos2d::Ref* /*sender*/, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			auto text_field = static_cast<ui::TextField*>( getChildByTag( TAG_TextField ) );
			if( text_field->getString().empty() )
			{
				CCLOG( "File Name : Empty" );
				return;
			}

			CCLOG( "File Save" );
			std::stringstream path;
			path << cocos2d::FileUtils::getInstance()->getWritablePath();
			path << text_field->getString().c_str();
			path << ".stage";

			tool::TerrainSaveHelper::Save( mTerrainData, path.str().c_str() );
		}
		void TerrainToolScene::onLoad( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			auto text_field = static_cast<ui::TextField*>( getChildByTag( TAG_TextField ) );
			if( text_field->getString().empty() )
			{
				CCLOG( "File Name : Empty" );
				return;
			}

			CCLOG( "File Load" );
			std::stringstream path;
			path << cocos2d::FileUtils::getInstance()->getWritablePath();
			path << text_field->getString().c_str();
			path << ".stage";
			mTerrainData.Load( path.str().c_str() );

			// apply terrain data
			mTerrainViewer->LoadTerrainData4Original( mTerrainData );
		}


		void TerrainToolScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	} // namespace game
}
