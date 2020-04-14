#include "ui_practice_Minimap.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCCamera.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"
#include "renderer/CCFrameBuffer.h"

#include "ui_practice_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_CaptureCamera = 20140416;
	const int TAG_MoveSpeedLabel = 20140417;
}

namespace ui_practice
{
	Minimap::Minimap() :
		mKeyboardListener( nullptr )
		, mCurrentPressedCount( 0 )
		, mCameraMoveVec2()
		, mCameraMoveSpeed( 5 )
	{}

	Scene* Minimap::create()
	{
		auto ret = new ( std::nothrow ) Minimap();
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

	bool Minimap::init()
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
			ss << std::endl;
			ss << "[1] : Increase Camera Speed";
			ss << std::endl;
			ss << "[2] : Decrease Camera Speed";

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
			auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 150 ) );
			addChild( background_layer, -1 );
		}

		//
		// Move Speed
		//
		{
			auto label = Label::createWithTTF( "", "fonts/arial.ttf", 9 );
			label->setTag( TAG_MoveSpeedLabel );
			label->setAnchorPoint( Vec2( 1.f, 0.f ) );
			label->setColor( Color3B::GREEN );
			label->setCameraMask( static_cast<unsigned short>( CameraFlag::USER1 ) );
			label->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width
				, visibleOrigin.y
			) );
			addChild( label, std::numeric_limits<int>::max() );

			updateMoveSpeedLabel();
		}

		//
		// Practice
		//
		{
			// Just Label
			{
				auto label = Label::createWithTTF( "Just Label", "fonts/arial.ttf", 9 );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( label );
			}

			// Capture Game View
			{
				const auto capture_view_size = visibleSize;

				// capture camera
				auto capture_camera = Camera::createOrthographic(
					capture_view_size.width, capture_view_size.height
					, 1, 1000
				);
				capture_camera->setTag( TAG_CaptureCamera );
				capture_camera->setCameraFlag( CameraFlag::DEFAULT );
				capture_camera->setDepth( 1 ); // important - CameraFlag::DEFAULT is 0, user defined camera is -1 by default
				capture_camera->setPositionZ( 1 );
				capture_camera->lookAt( Vec3( 0, 0, 0 ), Vec3( 0, 1, 0 ) );
				addChild( capture_camera );
				{
					// frame buffer
					auto frame_buffer = experimental::FrameBuffer::create( 1, capture_view_size.width, capture_view_size.height );
					{
						frame_buffer->setClearColor( Color4F::RED );
						capture_camera->setFrameBufferObject( frame_buffer );
					}

					// render target
					{
						auto render_target_normal = experimental::RenderTarget::create( capture_view_size.width, capture_view_size.height );
						render_target_normal->getTexture()->setAliasTexParameters();
						frame_buffer->attachRenderTarget( render_target_normal );
					}

					//stencil
					{
						auto render_target_depth_stencil = experimental::RenderTargetDepthStencil::create( capture_view_size.width, capture_view_size.height );
						frame_buffer->attachDepthStencilTarget( render_target_depth_stencil );
					}

					// view
					{
						Texture2D* texture = frame_buffer->getRenderTarget()->getTexture();
						auto sprite = Sprite::createWithTexture( texture );
						sprite->setAnchorPoint( Vec2::ONE );
						sprite->setScale( 0.5f );
						sprite->setCameraMask( static_cast<unsigned short>( CameraFlag::USER1 ) );
						sprite->setFlippedY( true );
						sprite->setPosition( Vec2(
							visibleOrigin.x + visibleSize.width
							, visibleOrigin.y + visibleSize.height
						) );
						addChild( sprite );
					}
				}
			}

			// UI Camera
			{
				auto ui_camera = Camera::createOrthographic(
					visibleSize.width, visibleSize.height
					, 1, 1000
				);
				ui_camera->setCameraFlag( CameraFlag::USER1 );
				ui_camera->setDepth( 2 );
				ui_camera->setPositionZ( 1 );
				ui_camera->lookAt( Vec3( 0, 0, 0 ), Vec3( 0, 1, 0 ) );
				addChild( ui_camera );
			}
		}

		return true;
	}

	void Minimap::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( Minimap::onKeyPressed, this );
		mKeyboardListener->onKeyReleased = CC_CALLBACK_2( Minimap::onKeyReleased, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void Minimap::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Node::onExit();
	}

	void Minimap::update( float dt )
	{
		if( 0 < mCurrentPressedCount )
		{
			if( 0.f < std::abs( mCameraMoveVec2.x ) || 0.f < std::abs( mCameraMoveVec2.y ) )
			{
				CCLOG( "x : %.2f, y : %.2f", mCameraMoveVec2.x, mCameraMoveVec2.y );
				const auto temp = mCameraMoveVec2 * mCameraMoveSpeed;

				getDefaultCamera()->setPosition3D( getDefaultCamera()->getPosition3D() + Vec3( -temp.x, -temp.y, 0 ) );
				for( auto c : getCameras() )
				{
					if( TAG_CaptureCamera == c->getTag() )
					{
						c->setPosition3D( c->getPosition3D() + Vec3( -temp.x, -temp.y, 0 ) );
						break;
					}
				}
			}
		}

		Scene::update( dt );
	}

	void Minimap::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
			return;
		}

		if( EventKeyboard::KeyCode::KEY_1 == keycode )
		{
			++mCameraMoveSpeed;
			updateMoveSpeedLabel();
		}
		else if( EventKeyboard::KeyCode::KEY_2 == keycode )
		{
			mCameraMoveSpeed = std::max( 1, mCameraMoveSpeed - 1 );
			updateMoveSpeedLabel();
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
	void Minimap::onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ )
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

	void Minimap::updateMoveSpeedLabel()
	{
		auto label = static_cast<Label*>( getChildByTag( TAG_MoveSpeedLabel ) );
		label->setString( StringUtils::format( "Move Speed : %d", mCameraMoveSpeed ) );
	}
}
