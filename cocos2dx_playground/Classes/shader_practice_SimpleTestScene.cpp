#include "shader_practice_SimpleTestScene.h"

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
#include "ui/UISlider.h"

#include "platform/CCFileUtils.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccShaders.h"

#include "shader_practice_GLUniformInfos.h"

USING_NS_CC;

namespace
{
	const char* CustomeShaderPath = "shaders/shader_practice_SimpleTestScene.fsh";

	class UniformHelperNode : public Node
	{
	public:
		using SliderChangeCallback = std::function<void( float )>;

	private:
		UniformHelperNode() {}

	public:
		static UniformHelperNode* create( const shader_practice::GLUniformInfos::Info& info, const float start_value, const SliderChangeCallback& slider_change_callback )
		{
			auto ret = new ( std::nothrow ) UniformHelperNode();
			if( !ret || !ret->init( info, start_value, slider_change_callback ) )
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

		bool init( const shader_practice::GLUniformInfos::Info& info, const float start_value, const SliderChangeCallback& slider_change_callback )
		{
			if( !Node::init() )
			{
				return false;
			}

			setContentSize( Size( 180.f, 80.f ) );
			const float Content_Scale( 4.f );
			const Vec2 CenterPosition( getContentSize() * 0.5f );
			const int StartPercent( static_cast<int>( start_value * 100.f ) );

			//
			// Pivot
			//
			{
				auto pivot_indicator = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot_indicator->setScale( Content_Scale );
				addChild( pivot_indicator );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 100, 100, 100, 150 ), getContentSize().width, getContentSize().height );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Title
			//
			{
				auto label = Label::createWithTTF( info.Name, "fonts/NanumSquareR.ttf", 10 );
				label->setColor( Color3B::ORANGE );
				label->setPosition(
					CenterPosition
					+ Vec2( 0.f, 15.f )
				);
				addChild( label );
			}

			//
			// Percent
			//
			auto percent_label = Label::createWithTTF( std::to_string( StartPercent ), "fonts/NanumSquareR.ttf", 10 );
			percent_label->setColor( Color3B::GREEN );
			percent_label->setPosition( CenterPosition );
			addChild( percent_label, 1 );

			//
			// Slider
			//
			auto slider = ui::Slider::create();
			slider->setScale( Content_Scale );
			slider->loadBarTexture( "ui_track_bar_01_0.png", ui::Widget::TextureResType::PLIST );
			slider->loadProgressBarTexture( "ui_track_bar_01_1.png", ui::Widget::TextureResType::PLIST );
			slider->loadSlidBallTextureNormal( "ui_track_ball_0.png", ui::Widget::TextureResType::PLIST );
			slider->loadSlidBallTexturePressed( "ui_track_ball_1.png", ui::Widget::TextureResType::PLIST );
			slider->loadSlidBallTextureDisabled( "ui_track_ball_2.png", ui::Widget::TextureResType::PLIST );
			slider->setPercent( StartPercent );
			slider->setPosition(
				CenterPosition
				- Vec2( 0.f, 15.f )
			);
			slider->addEventListener( [slider, percent_label, slider_change_callback]( Ref*, ui::Slider::EventType event_type )
			{
				if( ui::Slider::EventType::ON_PERCENTAGE_CHANGED != event_type )
				{
					return;
				}

				percent_label->setString( std::to_string( slider->getPercent() ) );

				slider_change_callback( 0.01f * slider->getPercent() );
			} );
			addChild( slider );
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setPosition( slider->getPosition() );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			return true;
		}
	};
}

namespace shader_practice
{
	SimpleTestScene::SimpleTestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* SimpleTestScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) SimpleTestScene( back_to_the_previous_scene_callback );
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

	bool SimpleTestScene::init()
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
		const float start_value = 0.5f;

		//
		// Practice : View + Custome Shader
		//
		auto view_node = Sprite::create( "textures/step_typetype/step_typetype_dummy_01.png" );
		{
			view_node->getTexture()->setAliasTexParameters();
			view_node->setPosition(
				visibleOrigin.x + visibleSize.width * 0.5f
				, visibleOrigin.y + visibleSize.height * 0.6f
			);
			view_node->setScale( 4.f );
			addChild( view_node );
			{
				const auto shader_source = cocos2d::FileUtils::getInstance()->getStringFromFile( cocos2d::FileUtils::getInstance()->fullPathForFilename( CustomeShaderPath ) );
				auto gl_program = cocos2d::GLProgram::createWithByteArrays( ccPositionTextureColor_noMVP_vert, shader_source.c_str() );

				auto gl_program_state = GLProgramState::getOrCreateWithGLProgram( gl_program );
				view_node->setGLProgramState( gl_program_state );

				gl_program_state->setUniformFloat( "u_testColor", start_value );

				//
				// Load GL Uniform List
				//
				uniform_list.Load( gl_program->getProgram() );
			}
		}

		//
		// Controller
		//
		{
			const auto& target_uniform = *uniform_list.GetContainer().begin();
			auto uniform_helper_node = UniformHelperNode::create(
				target_uniform
				, start_value
				, [target_uniform, view_node]( float new_value )
				{
					view_node->getGLProgramState()->setUniformFloat( target_uniform.Location, new_value );
				}
			);
			uniform_helper_node->setPosition(
				visibleOrigin.x + ( visibleSize.width * 0.5f ) - ( uniform_helper_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height * 0.1f
			);
			addChild( uniform_helper_node );
		}

		return true;
	}

	void SimpleTestScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SimpleTestScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void SimpleTestScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void SimpleTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
