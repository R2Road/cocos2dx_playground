#include "Step01_Tool_MapToolScene.h"

#include <new>
#include <sstream>
#include <array>

#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

#include "Step01_RootScene.h"

USING_NS_CC;

const int TAG_Indicator = 20140416;

namespace step01
{
	namespace tool
	{
		MapToolScene::MapToolScene() :
			mKeyboardListener( nullptr )
			, mTerrainData()
			, mCurrentTileType( step01::game::terrain::eTileType::road )
			, mButtonRootNode( nullptr )
			, terrain_layer( nullptr )
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
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Return to Step 01 Root";

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
			// Terrain Data
			//
			{
				const int map_size_x = 5;
				const int map_size_y = 5;
				mTerrainData.reSize( map_size_x, map_size_y );
			}

			//
			// Terrain View
			//
			{
				const auto tile_size = SpriteFrameCache::getInstance()->getSpriteFrameByName( "guide_01_1.png" )->getRect().size;
				const Vec2 pivot_position( tile_size.width * 0.5f, tile_size.height * 0.5f );
				terrain_layer = Node::create();
				terrain_layer->setContentSize( Size( tile_size.width * mTerrainData.getWidth(), tile_size.height * mTerrainData.getHeight() ) );
				terrain_layer->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - terrain_layer->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - terrain_layer->getContentSize().height ) * 0.5f )
				) );
				addChild( terrain_layer );

				ui::Button* temp = nullptr;
				int linear_index = 0;
				step01::game::terrain::eTileType tile_type = step01::game::terrain::eTileType::damage;
				for( int ty = 0; ty < mTerrainData.getHeight(); ++ty )
				{
					for( int tx = 0; tx < mTerrainData.getWidth(); ++tx )
					{
						linear_index = tx + ( mTerrainData.getHeight() * ty );

						temp = ui::Button::create( "guide_01_4.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
						temp->setTag( linear_index );
						{
							tile_type = mTerrainData.get( tx, ty );

							const auto& tile_data = step01::game::terrain::TileType2TileData( static_cast<step01::game::terrain::eTileType>( tile_type ) );

							auto indicator = Sprite::createWithSpriteFrameName( tile_data.ResourcePath );
							indicator->setTag( TAG_Indicator );
							indicator->setPosition( Vec2( temp->getContentSize().width * 0.5f, temp->getContentSize().height * 0.5f ) );
							temp->addChild( indicator );
						}
						temp->addTouchEventListener( CC_CALLBACK_2( MapToolScene::onButton, this ) );
						temp->setPosition( pivot_position + Vec2( ( tx * tile_size.width ), ( ty * tile_size.height ) ) );

						terrain_layer->addChild( temp );
					}
				}
			}

			//
			// ui
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
				indicator->setTag( TAG_Indicator );
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
					button_node->getChildByTag( TAG_Indicator )->setVisible( true );
				}
				else
				{
					button_node->getChildByTag( TAG_Indicator )->setVisible( false );
				}
			}
		}


		void MapToolScene::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			auto button = static_cast<Node*>( sender );

			const auto& tile_data = step01::game::terrain::TileType2TileData( static_cast<step01::game::terrain::eTileType>( mCurrentTileType ) );
			if( tile_data.bUnique )
			{
				int linear_index = 0;
				const auto& default_tile_data = step01::game::terrain::TileType2TileData( step01::game::terrain::eTileType::road );
				for( int ty = 0; ty < mTerrainData.getHeight(); ++ty )
				{
					for( int tx = 0; tx < mTerrainData.getWidth(); ++tx )
					{
						if( mTerrainData.get( tx, ty ) != mCurrentTileType )
						{
							continue;
						}
						
						mTerrainData.set( tx, ty, default_tile_data.TileType );

						linear_index = tx + ( mTerrainData.getHeight() * ty );
						auto indicator = static_cast<Sprite*>( terrain_layer->getChildByTag( linear_index )->getChildByTag( TAG_Indicator ) );
						indicator->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( default_tile_data.ResourcePath ) );
					}
				}
			}

			int y = button->getTag() / mTerrainData.getHeight();
			int x = button->getTag() - ( y * mTerrainData.getWidth() );
			mTerrainData.set( x, y, mCurrentTileType );

			auto indicator = static_cast<Sprite*>( button->getChildByTag( TAG_Indicator ) );
			indicator->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( tile_data.ResourcePath ) );
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
