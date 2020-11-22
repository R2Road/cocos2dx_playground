#include "shader_practice_GLUniformInfosScene.h"

#include <new>
#include <numeric>
#include <sstream>
#include <string>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "platform/CCFileUtils.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccShaders.h"

#include "shader_practice_GLUniformInfos.h"

USING_NS_CC;

namespace
{
	const char* CustomeShaderPath = "shaders/gl_uniform_infos_test.fsh";
}

namespace shader_practice
{
	GLUniformInfosScene::GLUniformInfosScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* GLUniformInfosScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) GLUniformInfosScene( back_to_the_previous_scene_callback );
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

	bool GLUniformInfosScene::init()
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
			ss << std::endl;
			ss << std::endl;
			ss << "[ESC] : Return to Root";

			auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 37, 18, 53, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// Custome Shader Path
		//
		{
			auto label_1 = Label::createWithTTF( CustomeShaderPath, "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
			label_1->setAnchorPoint( Vec2( 1.f, 1.f ) );
			label_1->setColor( Color3B::GREEN );
			label_1->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width
				, visibleOrigin.y + visibleSize.height
			) );
			addChild( label_1, std::numeric_limits<int>::max() );

			auto label_2 = Label::createWithTTF( "Custome Shader Path : ", "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
			label_2->setAnchorPoint( Vec2( 1.f, 1.f ) );
			label_2->setPosition(
				label_1->getPosition()
				- Vec2( label_1->getContentSize().width, 0.f )
			);
			addChild( label_2, std::numeric_limits<int>::max() );
		}

		GLUniformInfos uniform_list;

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
				const auto shader_source = cocos2d::FileUtils::getInstance()->getStringFromFile( cocos2d::FileUtils::getInstance()->fullPathForFilename( CustomeShaderPath ) );
				auto gl_program = cocos2d::GLProgram::createWithByteArrays( ccPositionTextureColor_noMVP_vert, shader_source.c_str() );

				auto gl_program_state = GLProgramState::getOrCreateWithGLProgram( gl_program );
				view_node->setGLProgramState( gl_program_state );

				gl_program_state->setUniformTexture( "u_texture2D", view_node->getTexture() );

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
			const Vec2 Margin( -2.f, 2.f );
			const Vec2 Spacing( 0.f, 20.f );
			const Vec2 NamePivotPosition( visibleOrigin.x + visibleSize.width * 0.25f, visibleOrigin.y );
			const Vec2 TypePivotPosition( visibleOrigin.x + visibleSize.width * 0.55f, visibleOrigin.y );
			const Vec2 LocationPivotPosition( visibleOrigin.x + visibleSize.width * 0.85f, visibleOrigin.y );

			int i = 0;
			Label* label = nullptr;
			std::string str;
			for( const auto& u : uniform_list.GetContainer() )
			{
				str = StringUtils::format( "Name : %-20s", u.Name.c_str() );
				label = Label::createWithTTF( str, "fonts/NanumSquareR.ttf", 9 );
				label->setAnchorPoint( Vec2( 0.f, 0.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					NamePivotPosition
					+ Margin
					+ ( Spacing * i )
				);
				addChild( label );

				str = StringUtils::format( "Type : %s( %d )", GLenum2String( u.Type ), u.Type );
				label = Label::createWithTTF( str, "fonts/NanumSquareR.ttf", 9 );
				label->setAnchorPoint( Vec2( 0.f, 0.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					TypePivotPosition
					+ Margin
					+ ( Spacing * i )
				);
				addChild( label );

				str = StringUtils::format( "Location : %d", u.Location );
				label = Label::createWithTTF( str, "fonts/NanumSquareR.ttf", 9 );
				label->setAnchorPoint( Vec2( 0.f, 0.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					LocationPivotPosition
					+ Margin
					+ ( Spacing * i )
				);
				addChild( label );

				++i;
			}
		}

		return true;
	}

	void GLUniformInfosScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GLUniformInfosScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void GLUniformInfosScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void GLUniformInfosScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
