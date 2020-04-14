#include "ui_practice_UIAndCamera.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCCamera.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "ui_practice_RootScene.h"

USING_NS_CC;

namespace ui_practice
{
	UIAndCamera::UIAndCamera() :
		mKeyboardListener( nullptr )
		, mCurrentPressedCount( 0 )
		, mCameraMoveVec2()
	{}

	Scene* UIAndCamera::create()
	{
		auto ret = new ( std::nothrow ) UIAndCamera();
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
			return nullptr;
		}
		else
		{
			ret->scheduleUpdate();
			ret->autorelease();
		}

		return ret;
	}

	bool UIAndCamera::init()
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
			ss << std::endl;
			ss << std::endl;
			ss << "[Arrow Key] : Move Default Camera";

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
			addChild( background_layer, -1 );
		}

		//
		// Practice
		//
		{
			// Just Label
			{
				auto label = Label::createWithTTF( "Just Label", "fonts/arial.ttf", 9 );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.25f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( label );
			}

			// Label with UI Camera
			{
				auto ui_camera = Camera::createOrthographic(
					visibleSize.width
					, visibleSize.height
					, 1
					, 1000
				);
				{
					ui_camera->setCameraFlag( CameraFlag::USER1 );
					ui_camera->setDepth( 1 ); // important - CameraFlag::DEFAULT is 0, user defined camera is -1 by default
					ui_camera->lookAt( Vec3( 0, 0, 0 ), Vec3( 0, 1, 0 ) );
					ui_camera->setPositionZ( 1 );
					addChild( ui_camera );
				}

				auto layer = LayerColor::create( Color4B( 0, 0, 255, 150 ), visibleSize.width * 0.5f, visibleSize.height );
				layer->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y
				) );
				addChild( layer );
				{
					auto label = Label::createWithTTF( "Label with UI Camera", "fonts/arial.ttf", 9 );
					label->setColor( Color3B::GREEN );
					label->setPosition( Vec2(
						layer->getContentSize().width * 0.5f
						, layer->getContentSize().height * 0.5f
					) );
					layer->addChild( label );
				}
				layer->setCameraMask( static_cast<unsigned short>( CameraFlag::USER1 ), true ); // important
			}
		}

		return true;
	}

	void UIAndCamera::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( UIAndCamera::onKeyPressed, this );
		mKeyboardListener->onKeyReleased = CC_CALLBACK_2( UIAndCamera::onKeyReleased, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void UIAndCamera::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Node::onExit();
	}

	void UIAndCamera::update( float dt )
	{
		if( 0 < mCurrentPressedCount )
		{
			if( 0.f < std::abs( mCameraMoveVec2.x ) || 0.f < std::abs( mCameraMoveVec2.y ) )
			{
				CCLOG( "x : %.2f, y : %.2f", mCameraMoveVec2.x, mCameraMoveVec2.y );
				const auto temp = mCameraMoveVec2 * 5.f;

				getDefaultCamera()->setPosition3D( getDefaultCamera()->getPosition3D() + Vec3( -temp.x, -temp.y, 0 ) );
			}
		}

		Scene::update( dt );
	}

	void UIAndCamera::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
			return;
		}

		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			mCameraMoveVec2.y += 1.f;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			mCameraMoveVec2.y -= 1.f;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			mCameraMoveVec2.x += 1.f;
			break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			mCameraMoveVec2.x -= 1.f;
			break;

		default:
			return;
		}

		++mCurrentPressedCount;
	}
	void UIAndCamera::onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event )
	{
		if( 0 == mCurrentPressedCount )
		{
			return;
		}

		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			mCameraMoveVec2.y -= 1.f;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			mCameraMoveVec2.y += 1.f;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			mCameraMoveVec2.x -= 1.f;
			break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			mCameraMoveVec2.x += 1.f;
			break;

		default:
			return;
		}

		--mCurrentPressedCount;
	}
}
