#include "shader_practice_GLUniformInfosScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "platform/CCFileUtils.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccShader_PositionTextureColor_noMVP.vert"

#include "shader_practice_GLUniformInfos.h"

USING_NS_CC;

namespace
{
	const char* FontPath = "fonts/arial.ttf";
}

namespace shader_practice
{
	GLUniformListScene::GLUniformListScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* GLUniformListScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) GLUniformListScene( back_to_the_previous_scene_callback );
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

	bool GLUniformListScene::init()
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

			auto label = Label::createWithTTF( ss.str(), FontPath, 9, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		GLUniformList uniform_list;

		//
		// Practice : View + Custome Shader
		//
		{
			auto view_node = Sprite::create( "textures/step_typetype/step_typetype_dummy_01.png" );
			view_node->getTexture()->setAliasTexParameters();
			view_node->setPosition(
				visibleOrigin.x + visibleSize.width * 0.5f
				, visibleOrigin.y + visibleSize.height * 0.5f
			);
			view_node->setScale( 4.f );
			addChild( view_node );
			{
				auto shader_source = cocos2d::FileUtils::getInstance()->getStringFromFile( cocos2d::FileUtils::getInstance()->fullPathForFilename( "shaders/gl_uniform_list.fsh" ) );
				auto gl_program = cocos2d::GLProgram::createWithByteArrays( ccPositionTextureColor_noMVP_vert, shader_source.c_str() );

				auto gl_program_state = GLProgramState::getOrCreateWithGLProgram( gl_program );
				view_node->setGLProgramState( gl_program_state );

				//
				// Load GL Uniform List
				//
				uniform_list.Load( gl_program->getProgram() );
			}
		}

		//
		// Practice : Uniform List
		//
		{
			const Vec2 margin( -2.f, -2.f );
			const Vec2 spacing( 0.f, -40.f );

			int i = 0;
			Label* label = nullptr;
			for( const auto& u : uniform_list.GetContainer() )
			{
				label = Label::createWithTTF( u.Name, FontPath, 9 );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					Vec2( visibleOrigin.x + visibleSize.width, visibleOrigin.y + visibleSize.height )
					+ margin
					+ ( spacing * i )
				);
				addChild( label );

				++i;
			}
		}

		return true;
	}

	void GLUniformListScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GLUniformListScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void GLUniformListScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Node::onExit();
	}


	void GLUniformListScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
