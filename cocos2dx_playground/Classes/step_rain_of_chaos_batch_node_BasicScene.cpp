#include "step_rain_of_chaos_batch_node_BasicScene.h"

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

namespace
{
	const int TAG_NormalNode = 20140416;
	const int TAG_BatchNode = 20160528;
	const int TAG_CurrentNodeTypeView = 100;
}

namespace step_rain_of_chaos
{
	namespace batch_node
	{
		BasicScene::BasicScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* BasicScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) BasicScene( back_to_the_previous_scene_callback );
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

		bool BasicScene::init()
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
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[1] : Show Normal Node";
				ss << cpg::linefeed;
				ss << "[2] : Show Batch Node";

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
				ss << "SpriteBatchNode reduces Draw Call and Something.";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "but Cocos2d-x has Auto Batching";
				ss << cpg::linefeed;
				ss << " - Go : Renderer::processRenderCommand ";
				ss << cpg::linefeed;
				ss << " - Trace : _queuedTriangleCommands";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "For that reason, it is not necessary to use it.";

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
			// Current Node Type View
			//
			{
				auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 10 );
				label->setTag( TAG_CurrentNodeTypeView );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.6f )
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Draw Call Indicator
			//
			{
				auto label = Label::createWithTTF( "<==== Same Draw Call", cpg::StringTable::GetFontPath(), 10 );
				label->setAnchorPoint( Vec2( 1.f, 0.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y
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

				const std::array<SpriteFrame*, 2u> SpriteFrames{
					SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_0.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_1.png" )
				};
				const Size SpriteSize = SpriteFrames[0]->getOriginalSizeInPixels();
				const int VerticalAmount = 3;
				const int HorizontalAmount = 3;


				//
				// Build Normal node
				//
				{
					auto normal_node = Node::create();
					normal_node->setTag( TAG_NormalNode );
					normal_node->setContentSize( Size( SpriteSize.width * VerticalAmount, SpriteSize.height * HorizontalAmount ) );
					normal_node->setPosition(
						nodePivot.x - ( normal_node->getContentSize().width * 0.5f )
						, nodePivot.y - ( normal_node->getContentSize().height * 0.5f )
					);
					addChild( normal_node );


					Sprite* temp_sprite = nullptr;
					auto sprite_frame_indicator = SpriteFrames.begin();
					for( int sy = 0; HorizontalAmount > sy; ++sy )
					{
						for( int sx = 0; VerticalAmount > sx; ++sx )
						{
							// generate sprite
							temp_sprite = Sprite::createWithSpriteFrame( *sprite_frame_indicator );
							temp_sprite->setScale( _director->getContentScaleFactor() );
							temp_sprite->setPosition(
								( SpriteSize.width * 0.5f ) + ( SpriteSize.width * sx )
								, ( SpriteSize.height * 0.5f ) + ( SpriteSize.height * sy )
							);
							normal_node->addChild( temp_sprite );

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
				// Build Sprite Batch node
				//
				{
					auto sprite_batch_node = SpriteBatchNode::create( "textures/texture_001.png", VerticalAmount * HorizontalAmount );
					sprite_batch_node->setTag( TAG_BatchNode );
					sprite_batch_node->setContentSize( Size( SpriteSize.width * VerticalAmount, SpriteSize.height * HorizontalAmount ) );
					sprite_batch_node->setPosition(
						nodePivot.x - ( sprite_batch_node->getContentSize().width * 0.5f )
						, nodePivot.y - ( sprite_batch_node->getContentSize().height * 0.5f )
					);
					addChild( sprite_batch_node );


					Sprite* temp_sprite = nullptr;
					auto sprite_frame_indicator = ++SpriteFrames.begin();
					for( int sy = 0; HorizontalAmount > sy; ++sy )
					{
						for( int sx = 0; VerticalAmount > sx; ++sx )
						{
							// generate sprite
							temp_sprite = Sprite::createWithSpriteFrame( *sprite_frame_indicator );
							temp_sprite->setScale( _director->getContentScaleFactor() );
							temp_sprite->setPosition(
								( SpriteSize.width * 0.5f ) + ( SpriteSize.width * sx )
								, ( SpriteSize.height * 0.5f ) + ( SpriteSize.height * sy )
							);
							sprite_batch_node->addChild( temp_sprite );

							// update target sprite frame
							++sprite_frame_indicator;
							if( SpriteFrames.end() == sprite_frame_indicator )
							{
								sprite_frame_indicator = SpriteFrames.begin();
							}
						}
					}
				}

				selectTestNode( TAG_BatchNode );
			}

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void BasicScene::selectTestNode( const int node_tag )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_CurrentNodeTypeView ) );

			if( TAG_NormalNode == node_tag )
			{
				label->setString( "Normal Node" );

				getChildByTag( TAG_NormalNode )->setVisible( true );
				getChildByTag( TAG_BatchNode )->setVisible( false );
			}
			else if( TAG_BatchNode == node_tag )
			{
				label->setString( "Batch Node" );

				getChildByTag( TAG_NormalNode )->setVisible( false );
				getChildByTag( TAG_BatchNode )->setVisible( true );
			}
		}

		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_1:
				selectTestNode( TAG_NormalNode );
				return;
			case EventKeyboard::KeyCode::KEY_2:
				selectTestNode( TAG_BatchNode );
				return;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
