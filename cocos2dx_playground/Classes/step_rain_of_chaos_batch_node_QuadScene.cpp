#include "step_rain_of_chaos_batch_node_QuadScene.h"

#include <array>
#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteBatchNode.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace batch_node
	{
		QuadScene::QuadScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* QuadScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) QuadScene( back_to_the_previous_scene_callback );
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

		bool QuadScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleOrigin = _director->getVisibleOrigin();
			const auto visibleSize = _director->getVisibleSize();

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 130, 49, 29, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Explain
			//
			{
				std::stringstream ss;
				ss << "Use SpriteBatchNode::insertQuadFromSprite instead addChild";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Batch Node Test
			//
			{
				const Vec2 nodePivot(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.4f )
				);

				const std::array<SpriteFrame*, 4u> SpriteFrames{
					SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_0.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_1.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_2.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_3.png" )
				};
				const Size SpriteSize = SpriteFrames[0]->getOriginalSizeInPixels();
				const int VerticalAmount = 7;
				const int HorizontalAmount = 7;


				// generate sprite batch node
				auto sprite_batch_node = SpriteBatchNode::create( "textures/texture_001.png", VerticalAmount * HorizontalAmount );
				sprite_batch_node->setContentSize( Size( SpriteSize.width * VerticalAmount, SpriteSize.height * HorizontalAmount ) );
				sprite_batch_node->setPosition(
					nodePivot.x - ( sprite_batch_node->getContentSize().width * 0.5f )
					, nodePivot.y - ( sprite_batch_node->getContentSize().height * 0.5f )
				);
				addChild( sprite_batch_node );

				//
				// Build
				//
				{
					// generate sprite
					Sprite* temp_sprite = Sprite::create();
					temp_sprite->setScale( _director->getContentScaleFactor() );

					auto sprite_frame_indicator = ++SpriteFrames.begin();
					for( int sy = 0; HorizontalAmount > sy; ++sy )
					{
						for( int sx = 0; VerticalAmount > sx; ++sx )
						{
							temp_sprite->setSpriteFrame( *sprite_frame_indicator );
							temp_sprite->setPosition(
								( SpriteSize.width * 0.5f ) + ( SpriteSize.width * sx )
								, ( SpriteSize.height * 0.5f ) + ( SpriteSize.height * sy )
							);
							sprite_batch_node->insertQuadFromSprite( temp_sprite, sx + ( HorizontalAmount * sy ) );

							// update target sprite frame
							++sprite_frame_indicator;
							if( SpriteFrames.end() == sprite_frame_indicator )
							{
								sprite_frame_indicator = SpriteFrames.begin();
							}
						}
					}
				}

				//
				// Child Count View
				//
				{
					auto label = Label::createWithTTF( StringUtils::format( "Child Count : %d", sprite_batch_node->getChildrenCount() ), cpg::StringTable::GetFontPath(), 10 );
					label->setPosition( Vec2(
						visibleOrigin.x + ( visibleSize.width * 0.5f )
						, visibleOrigin.y + ( visibleSize.height * 0.8f )
					) );
					addChild( label, std::numeric_limits<int>::max() );
				}
			}

			return true;
		}

		void QuadScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( QuadScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void QuadScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void QuadScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
