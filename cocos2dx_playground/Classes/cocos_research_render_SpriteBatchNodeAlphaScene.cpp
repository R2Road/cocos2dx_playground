#include "cocos_research_render_SpriteBatchNodeAlphaScene.h"

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

#include "platform/CCFileUtils.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccShaders.h"

#include "cpg_Clamp.h"
#include "cpg_node_PivotNode.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "shader_practice_GLUniformInfos.h"

USING_NS_CC;

namespace
{
	const char* CustomeShaderPath = "shaders/cocos_research_render_SpriteBatchNodeAlphaScene.fsh";
	const char* CustomeShaderName = "shaders/cocos_research_render_SpriteBatchNodeAlphaScene";
}

namespace cocos_research_render
{
	SpriteBatchNodeAlphaScene::SpriteBatchNodeAlphaScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mSpriteBatchNode( nullptr )
		, mTileMapOpacityLabel( nullptr )
	{}

	Scene* SpriteBatchNodeAlphaScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) SpriteBatchNodeAlphaScene( back_to_the_previous_scene_callback );
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

	bool SpriteBatchNodeAlphaScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleSize = _director->getVisibleSize();
		const auto visibleOrigin = _director->getVisibleOrigin();
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

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 8 );
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
			auto background_layer = LayerColor::create( Color4B( 37, 18, 53, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// 
		//
		{
			mTileMapOpacityLabel = Label::createWithTTF( "---", cpg::StringTable::GetFontPath(), 8 );
			mTileMapOpacityLabel->setAnchorPoint( Vec2( 1.f, 1.f ) );
			mTileMapOpacityLabel->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width, visibleSize.height )
			);
			addChild( mTileMapOpacityLabel, std::numeric_limits<int>::max() );
		}

		//
		// Practice : Load and Caching
		//
		{
			// Load
			const auto shader_source = FileUtils::getInstance()->getStringFromFile( FileUtils::getInstance()->fullPathForFilename( CustomeShaderPath ) );
			auto gl_program = GLProgram::createWithByteArrays( ccPositionTextureColor_vert, shader_source.c_str() );

			// Caching
			GLProgramCache::getInstance()->addGLProgram( gl_program, CustomeShaderName );
		}

		//
		// Research
		//
		{
			//
			// Batch-Node
			//
			{
				Sprite* temp_sprite = Sprite::create( "textures/algorithm_practice/algorithm_practice_tilesheet_01.png" );
				temp_sprite->getTexture()->setAliasTexParameters();
				temp_sprite->setScale( _director->getContentScaleFactor() );


				mSpriteBatchNode = SpriteBatchNode::create( "textures/algorithm_practice/algorithm_practice_tilesheet_01.png", 2 );
				mSpriteBatchNode->setContentSize( temp_sprite->getContentSize() );
				mSpriteBatchNode->setPosition( Vec2(
					visibleCenter.x - ( mSpriteBatchNode->getContentSize().width * 0.5f )
					, visibleCenter.y - ( mSpriteBatchNode->getContentSize().height * 0.5f )
				) );
				addChild( mSpriteBatchNode );

				mSpriteBatchNode->insertQuadFromSprite( temp_sprite, 0 );

				//
				//
				//
				{
					shader_practice::GLUniformInfos uniform_list;

					//
					// Get Cached Program
					//
					auto gl_program = GLProgramCache::getInstance()->getGLProgram( CustomeShaderName );
					uniform_list.Load( gl_program->getProgram() );
					CCASSERT( !uniform_list.GetContainer().empty(), "" );

					//
					// Create GLProgramState
					//
					auto gl_program_state = GLProgramState::getOrCreateWithGLProgram( gl_program );

					//
					// Apply
					// 이거 왜 적용이 안되냐? 진짜 그지 같이 만들어놨네.
					//
					mSpriteBatchNode->setGLProgramState( gl_program_state );
					gl_program_state->setUniformFloat( "u_customAlpha", 0.5f );

				}
			}

			//
			// Pivot
			//
			{
				auto pivot_node = cpg_node::PivotNode::create();
				pivot_node->setPosition( mSpriteBatchNode->getPosition() );
				addChild( pivot_node, std::numeric_limits<int>::max() );
			}

		}

		//
		//
		//
		mTileMapOpacityLabel->setString( std::to_string( mSpriteBatchNode->getOpacity() ) );

		return true;
	}

	void SpriteBatchNodeAlphaScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SpriteBatchNodeAlphaScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void SpriteBatchNodeAlphaScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void SpriteBatchNodeAlphaScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
}
