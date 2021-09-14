#include "ui_practice_SliderScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"
#include "ui/UISlider.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace ui_practice
{
	SliderScene::SliderScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* SliderScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) SliderScene( back_to_the_previous_scene_callback );
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

	bool SliderScene::init()
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
			auto background_layer = LayerColor::create( Color4B( 50, 75, 112, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		const float Content_Scale( 4.f );

		//
		// Practice : Slider Normal
		//
		{
			auto slider = ui::Slider::create();
			slider->setScale( Content_Scale );
			slider->loadBarTexture( "ui_track_bar_01_0.png", ui::Widget::TextureResType::PLIST );
			slider->loadProgressBarTexture( "ui_track_bar_01_1.png", ui::Widget::TextureResType::PLIST );
			slider->loadSlidBallTextureNormal( "ui_track_ball_0.png", ui::Widget::TextureResType::PLIST );
			slider->loadSlidBallTexturePressed( "ui_track_ball_1.png", ui::Widget::TextureResType::PLIST );
			slider->loadSlidBallTextureDisabled( "ui_track_ball_2.png", ui::Widget::TextureResType::PLIST );
			slider->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width * 0.25f
				, visibleOrigin.y + visibleSize.height * 0.25f
			) );
			addChild( slider );

			auto label = Label::createWithTTF( "Slider : Normal", cpg::StringTable::GetFontPath(), 10 );
			label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
			label->setColor( Color3B::GREEN );
			label->setPosition( slider->getPosition() - Vec2( 0.f, 20.f ) );
			addChild( label );
		}

		//
		// Practice : Slider With Scale9Enable
		//
		{
			auto slider = ui::Slider::create();
			slider->loadBarTexture( "ui_track_bar_01_0.png", ui::Widget::TextureResType::PLIST );
			slider->loadProgressBarTexture( "ui_track_bar_01_1.png", ui::Widget::TextureResType::PLIST );
			slider->loadSlidBallTextureNormal( "ui_track_ball_0.png", ui::Widget::TextureResType::PLIST );
			slider->loadSlidBallTexturePressed( "ui_track_ball_1.png", ui::Widget::TextureResType::PLIST );
			slider->loadSlidBallTextureDisabled( "ui_track_ball_2.png", ui::Widget::TextureResType::PLIST );
			slider->setScale9Enabled( true ); // Warning : has sequence problem
			slider->setContentSize( Size( 22, 4 ) );
			slider->setScale( Content_Scale );
			slider->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width * 0.5f
				, visibleOrigin.y + visibleSize.height * 0.5f
			) );
			addChild( slider );

			slider->setPercent( 10 );

			auto label = Label::createWithTTF( "Slider : With Scale9Enable\n\n ...mm Not Good", cpg::StringTable::GetFontPath(), 10 );
			label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
			label->setColor( Color3B::GREEN );
			label->setPosition( slider->getPosition() - Vec2( 0.f, 20.f ) );
			addChild( label );
		}

		//
		// Practice : Slider + @
		//
		{
			auto root_node = Node::create();
			root_node->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width * 0.75f
				, visibleOrigin.y + visibleSize.height * 0.75f
			) );
			addChild( root_node );
			{
				// Percent
				auto percent_label = Label::createWithTTF( "slider : 0", cpg::StringTable::GetFontPath(), 10 );
				percent_label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				percent_label->setColor( Color3B::ORANGE );
				percent_label->setPosition( root_node->getPosition() + Vec2( 0.f, 20.f ) );
				addChild( percent_label );

				// Slider
				{
					auto slider = ui::Slider::create();
					slider->setScale( Content_Scale );
					slider->loadBarTexture( "ui_track_bar_01_0.png", ui::Widget::TextureResType::PLIST );
					slider->loadProgressBarTexture( "ui_track_bar_01_1.png", ui::Widget::TextureResType::PLIST );
					slider->loadSlidBallTextureNormal( "ui_track_ball_0.png", ui::Widget::TextureResType::PLIST );
					slider->loadSlidBallTexturePressed( "ui_track_ball_1.png", ui::Widget::TextureResType::PLIST );
					slider->loadSlidBallTextureDisabled( "ui_track_ball_2.png", ui::Widget::TextureResType::PLIST );
					slider->addEventListener( [slider, percent_label]( Ref*, ui::Slider::EventType event_type ) {
						if( ui::Slider::EventType::ON_PERCENTAGE_CHANGED != event_type )
						{
							return;
						}

						percent_label->setString( StringUtils::format(
							"slider : %d"
							, slider->getPercent()
						) );
					} );
					root_node->addChild( slider );
				}

				// Background
				{
					auto background_sprite = Sprite::createWithSpriteFrameName( "ui_track_bar_bg_0.png" );
					background_sprite->setScale( Content_Scale );
					root_node->addChild( background_sprite, -1 );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Slider : + @", cpg::StringTable::GetFontPath(), 10 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition( root_node->getPosition() - Vec2( 0.f, 20.f ) );
					addChild( label );
				}
			}
		}

		return true;
	}

	void SliderScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SliderScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void SliderScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void SliderScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
