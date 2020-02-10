#include "Step01_Tool_MapToolScene.h"

#include <new>
#include <sstream>
#include <array>

#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UITextField.h"

#include "Step01_Tool_TerrainViewer.h"

#include "Step01_RootScene.h"

USING_NS_CC;

const int TAG_TextField = 9999;
const int TAG_SelectedTile_Indicator = 20140416;

namespace step01
{
	namespace tool
	{
		MapToolScene::MapToolScene() :
			mKeyboardListener( nullptr )
			, mTerrainData()
			, mCurrentTileType( step01::game::terrain::eTileType::road )
			, mButtonRootNode( nullptr )
			, mTerrainViewer( nullptr )
		{}

		Scene* MapToolScene::create()
		{
			auto ret = new ( std::nothrow ) MapToolScene();
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

		bool MapToolScene::init()
		{
			if( !Scene::init() )
				return false;

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
				ss << "[ESC] : Return to Step 01 Root";
				ss << std::endl;
				ss << std::endl;
				ss << "<File Path : Save n Load> : " << cocos2d::FileUtils::getInstance()->getWritablePath();

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}

			//
			// Terrain View
			//
			{
				mTerrainViewer = TerrainViewer::create( mTerrainData.getWidth(), mTerrainData.getHeight(), CC_CALLBACK_2( MapToolScene::onGrid, this ) );
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
				mButtonRootNode = Node::create();
				addChild( mButtonRootNode );

				const auto tile_select_callback = CC_CALLBACK_2( MapToolScene::onTileSelect, this );

				int by = 0;
				for( int cur = static_cast<int>( step01::game::terrain::eTileType::FIRST ), end = static_cast<int>( step01::game::terrain::eTileType::SIZE ); cur < end; ++cur )
				{
					const auto& tile_data = step01::game::terrain::TileType2TileData( static_cast<step01::game::terrain::eTileType>( cur ) );
					if( !tile_data.bToolEnable )
					{
						continue;
					}

					auto button = makeMenuButton(
						tile_data.TileType
						, tile_data.Name
						, tile_select_callback
					);
					button->setPosition( Vec2(
						visibleOrigin.x + ( button->getContentSize().width * 0.5f )
						, visibleOrigin.y + ( visibleSize.height * 0.5f ) + ( button->getContentSize().height * by )
					) );
					mButtonRootNode->addChild( button );

					++by;
				}

				onTileSelect(
					mButtonRootNode->getChildByTag( static_cast<int>( step01::game::terrain::eTileType::road ) )
					, ui::Widget::TouchEventType::BEGAN
				);
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
				save_button->addTouchEventListener( CC_CALLBACK_2( MapToolScene::onSave, this ) );
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
				save_button->addTouchEventListener( CC_CALLBACK_2( MapToolScene::onLoad, this ) );
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

		void MapToolScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( MapToolScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void MapToolScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}


		Node* MapToolScene::makeMenuButton( const step01::game::terrain::eTileType tile_type, const char* button_text, const ui::Widget::ccWidgetTouchCallback& callback )
		{
			const Size button_margin( 10.f, 4.f );

			auto button = ui::Button::create( "guide_01_4.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
			button->setTag( static_cast<int>( tile_type ) );
			button->addTouchEventListener( callback );
			button->setScale9Enabled( true );
			{
				auto label = Label::createWithTTF( button_text, "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				button->setTitleLabel( label );
				button->setContentSize( label->getContentSize() + button_margin + button_margin );

				auto indicator = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
				indicator->setAnchorPoint( Vec2( 0.f, 0.f ) );
				indicator->setTag( TAG_SelectedTile_Indicator );
				indicator->setContentSize( button->getContentSize() );
				button->addChild( indicator );
			}

			return button;
		}


		void MapToolScene::onTileSelect( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			// change current tile type
			auto sender_node = static_cast<Node*>( sender );
			mCurrentTileType = static_cast<step01::game::terrain::eTileType>( sender_node->getTag() );

			// setup indicator visibility
			Node* button_node = nullptr;
			for( int cur = static_cast<int>( step01::game::terrain::eTileType::FIRST ), end = static_cast<int>( step01::game::terrain::eTileType::SIZE ); cur < end; ++cur )
			{
				button_node = mButtonRootNode->getChildByTag( cur );
				if( !button_node )
				{
					continue;
				}

				if( cur == static_cast<int>( mCurrentTileType ) )
				{
					button_node->getChildByTag( TAG_SelectedTile_Indicator )->setVisible( true );
				}
				else
				{
					button_node->getChildByTag( TAG_SelectedTile_Indicator )->setVisible( false );
				}
			}
		}


		void MapToolScene::onGrid( Ref* sender, ui::Widget::TouchEventType touch_event_type )
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

			if( step01::game::terrain::TileType2UniqueFlag( mCurrentTileType ) )
			{
				const auto default_tile_type = step01::game::terrain::eTileType::road;
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


		void MapToolScene::onSave( cocos2d::Ref* /*sender*/, ui::Widget::TouchEventType touch_event_type )
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
		void MapToolScene::onLoad( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type )
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
			mTerrainData.load( text_field->getString().c_str() );

			// apply terrain data
			for( int ty = 0; ty < mTerrainData.getHeight(); ++ty )
			{
				for( int tx = 0; tx < mTerrainData.getWidth(); ++tx )
				{
					mTerrainViewer->UpdateTile( tx, ty, mTerrainData.get( tx, ty ) );
				}
			}
		}


		void MapToolScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step01::RootScene::create() );
		}
		void MapToolScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE != keycode )
			{
				return;
			}

			if( isScheduled( schedule_selector( MapToolScene::updateForExit ) ) )
			{
				return;
			}

			scheduleOnce( schedule_selector( MapToolScene::updateForExit ), 0.f );
		}
	} // namespace game
}
