#include "step_pathfinder_tool_TerrainToolScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "platform/CCFileUtils.h"
#include "ui/UIButton.h"
#include "ui/UITextField.h"

#include "step_pathfinder_tool_TerrainViewer.h"
#include "step_pathfinder_tool_ui_TileSelectNode.h"

#include "step_pathfinder_RootScene.h"

USING_NS_CC;

const int TAG_TextField = 20140416;

namespace step_pathfinder
{
	namespace tool
	{
		TerrainToolScene::TerrainToolScene() :
			mKeyboardListener( nullptr )
			, mTerrainData()
			, mCurrentTileType( step_pathfinder::game::terrain::eTileType::road )
			, mTerrainViewer( nullptr )
		{}

		Scene* TerrainToolScene::create()
		{
			auto ret = new ( std::nothrow ) TerrainToolScene();
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

		bool TerrainToolScene::init()
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
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << "<File Path : Save n Load> : " << cocos2d::FileUtils::getInstance()->getWritablePath();

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Terrain View
			//
			{
				mTerrainViewer = TerrainViewer::create( mTerrainData.getWidth(), mTerrainData.getHeight(), CC_CALLBACK_2( TerrainToolScene::onGrid, this ) );
				mTerrainViewer->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - mTerrainViewer->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - mTerrainViewer->getContentSize().height ) * 0.7f )
				) );
				addChild( mTerrainViewer );

				// apply terrain data
				for( int ty = 0; ty < mTerrainData.getHeight(); ++ty )
				{
					for( int tx = 0; tx < mTerrainData.getWidth(); ++tx )
					{
						mTerrainViewer->UpdateTile( tx, ty, mTerrainData.get( tx, ty ) );
					}
				}
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
			}

			//
			// ui - file name input
			//
			{
				const int TEXT_FIELD_MAX_LENGTH = 20;
				const char TEXT_FIELD_CURSOR_CHAR = 95; // "_"
				const std::string DUMMY_STRING( TEXT_FIELD_MAX_LENGTH, 'A' );
				const std::string PLACE_HOLDER_STRING( "input file name here~!" );

				auto ui_text_field = ui::TextField::create( DUMMY_STRING, "fonts/arial.ttf", 9 );
				ui_text_field->setTag( TAG_TextField );
				ui_text_field->setPlaceHolderColor( Color3B::GREEN );
				ui_text_field->setMaxLength( TEXT_FIELD_MAX_LENGTH );
				ui_text_field->setMaxLengthEnabled( true );
				ui_text_field->setCursorChar( TEXT_FIELD_CURSOR_CHAR );
				ui_text_field->setCursorEnabled( true );
				ui_text_field->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.28f )
				) );
				addChild( ui_text_field, 0 );

				auto guide_button = ui::Button::create( "guide_01_4.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				guide_button->setScale9Enabled( true );
				guide_button->setContentSize( ui_text_field->getContentSize() + Size( 20.f, 10.f ) );
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
				auto save_button = ui::Button::create( "guide_01_4.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				save_button->setScale9Enabled( true );
				save_button->addTouchEventListener( CC_CALLBACK_2( TerrainToolScene::onSave, this ) );
				save_button->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f ) - save_button->getContentSize().width
					, visibleOrigin.y + ( visibleSize.height * 0.2f )
				) );
				addChild( save_button );
				{
					auto label = Label::createWithTTF( "Save", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
					label->setColor( Color3B::RED );
					label->setPosition( Vec2(
						visibleOrigin.x
						, visibleOrigin.y + visibleSize.height
					) );
					save_button->setTitleLabel( label );
					save_button->setContentSize( label->getContentSize() + Size( 20.f, 10.f ) );
				}
			}

			//
			// ui - file load
			//
			{
				auto save_button = ui::Button::create( "guide_01_4.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				save_button->setScale9Enabled( true );
				save_button->addTouchEventListener( CC_CALLBACK_2( TerrainToolScene::onLoad, this ) );
				save_button->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f ) + save_button->getContentSize().width
					, visibleOrigin.y + ( visibleSize.height * 0.2f )
				) );
				addChild( save_button );
				{
					auto label = Label::createWithTTF( "Load", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
					label->setColor( Color3B::MAGENTA );
					label->setPosition( Vec2(
						visibleOrigin.x
						, visibleOrigin.y + visibleSize.height
					) );
					save_button->setTitleLabel( label );
					save_button->setContentSize( label->getContentSize() + Size( 20.f, 10.f ) );
				}
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

			Node::onExit();
		}


		Node* TerrainToolScene::makeMenuButton( const cocos2d::Size menu_size, const step_pathfinder::game::terrain::eTileType tile_type, const char* button_text, const ui::Widget::ccWidgetTouchCallback& callback )
		{
			auto button = ui::Button::create( "guide_01_4.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
			button->setTag( static_cast<int>( tile_type ) );
			button->addTouchEventListener( callback );
			button->setScale9Enabled( true );
			button->setContentSize( menu_size );
			{
				auto label = Label::createWithTTF( button_text, "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				button->setTitleLabel( label );
			}

			return button;
		}


		void TerrainToolScene::onTileSelect( const step_pathfinder::game::terrain::eTileType new_tile_type )
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
			const int gy = button->getTag() / mTerrainData.getHeight();
			const int gx = button->getTag() - ( gy * mTerrainData.getWidth() );
			if( mCurrentTileType == mTerrainData.get( gx, gy ) )
			{
				return;
			}

			if( step_pathfinder::game::terrain::TileType2UniqueFlag( mCurrentTileType ) )
			{
				const auto default_tile_type = step_pathfinder::game::terrain::eTileType::road;
				for( int ty = 0; ty < mTerrainData.getHeight(); ++ty )
				{
					for( int tx = 0; tx < mTerrainData.getWidth(); ++tx )
					{
						if( mTerrainData.get( tx, ty ) != mCurrentTileType )
						{
							continue;
						}
						
						mTerrainData.set( tx, ty, default_tile_type );
						mTerrainViewer->UpdateTile( tx, ty, default_tile_type );
					}
				}
			}

			
			mTerrainData.set( gx, gy, mCurrentTileType );
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

			mTerrainData.save( path.str().c_str() );
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
			mTerrainData.load( path.str().c_str() );

			// apply terrain data
			for( int ty = 0; ty < mTerrainData.getHeight(); ++ty )
			{
				for( int tx = 0; tx < mTerrainData.getWidth(); ++tx )
				{
					mTerrainViewer->UpdateTile( tx, ty, mTerrainData.get( tx, ty ) );
				}
			}
		}


		void TerrainToolScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( step_pathfinder::RootScene::create() );
				return;
			}
		}
	} // namespace game
}
