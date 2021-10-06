#include "shader_practice_CCRandom01Scene.h"

#include <new>
#include <numeric>
#include <string>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "platform/CCFileUtils.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccShaders.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const char* CustomeShaderPath = "shaders/shader_practice_CCRandom01Scene.fsh";
}

namespace shader_practice
{
	CCRandom01Scene::CCRandom01Scene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* CCRandom01Scene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) CCRandom01Scene( back_to_the_previous_scene_callback );
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

	bool CCRandom01Scene::init()
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

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 9, Size::ZERO, TextHAlignment::LEFT );
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
		// Custome Shader Path
		//
		{
			auto label_1 = Label::createWithTTF( CustomeShaderPath, cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			label_1->setAnchorPoint( Vec2( 1.f, 1.f ) );
			label_1->setColor( Color3B::GREEN );
			label_1->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width, visibleSize.height )
			);
			addChild( label_1, std::numeric_limits<int>::max() );

			auto label_2 = Label::createWithTTF( "Custome Shader Path : ", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			label_2->setAnchorPoint( Vec2( 1.f, 1.f ) );
			label_2->setPosition(
				label_1->getPosition()
				- Vec2( label_1->getContentSize().width, 0.f )
			);
			addChild( label_2, std::numeric_limits<int>::max() );
		}

		//
		// Practice : Load N Caching
		//
		{
			// Load
			const auto shader_source = FileUtils::getInstance()->getStringFromFile( FileUtils::getInstance()->fullPathForFilename( CustomeShaderPath ) );
			auto gl_program = GLProgram::createWithByteArrays( ccPositionTextureColor_noMVP_vert, shader_source.c_str() );

			// Caching
			GLProgramCache::getInstance()->addGLProgram( gl_program, CustomeShaderPath );
		}
		//
		// Practice : Apply Custome Shader
		//
		{
			auto view_node = Sprite::create( "textures/step_typetype/step_typetype_dummy_01.png" );
			view_node->getTexture()->setAliasTexParameters();
			view_node->setPosition( visibleCenter );
			view_node->setScale( 2.f );
			addChild( view_node );
			{
				//
				// Get Cached Program
				//
				auto gl_program = GLProgramCache::getInstance()->getGLProgram( CustomeShaderPath );

				//
				// Create GLProgramState
				//
				auto gl_program_state = GLProgramState::getOrCreateWithGLProgram( gl_program );

				//
				// Apply
				//
				view_node->setGLProgramState( gl_program_state );
			}

			// Explain
			{
				auto label = Label::createWithTTF( "CC_Random01", cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::CENTER );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					view_node->getPosition()
					+ Vec2( 0.f, view_node->getContentSize().height * 0.5f )
					+ Vec2( 0.f, 50.f )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}
		}

		return true;
	}

	void CCRandom01Scene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( CCRandom01Scene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void CCRandom01Scene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void CCRandom01Scene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
