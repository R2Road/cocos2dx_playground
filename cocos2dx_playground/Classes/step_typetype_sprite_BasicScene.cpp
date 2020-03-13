#include "step_typetype_sprite_BasicScene.h"

#include <sstream>

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace
{
	const char* PLIST_Path = "textures/step_typetype/step_typetype_textures.plist";
	const char* TEXTURE_Path = "textures/step_typetype/step_typetype_textures.png";
}

namespace step_typetype
{
	namespace sprite
	{
		BasicScene::BasicScene() : mPressedKeyCount( 0 ), mKeyboardListener( nullptr ) {}

		Scene* BasicScene::create()
		{
			auto ret = new ( std::nothrow ) BasicScene();
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

		bool BasicScene::init()
		{
			if( !Scene::init() )
				return false;

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();


			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ Input - Key Code View";
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setPosition( Vec2(
					visibleOrigin.x + ( label->getContentSize().width * 0.5f )
					, visibleOrigin.y + visibleSize.height - ( label->getContentSize().height * 0.5f )
				) );
				addChild( label, 9999 );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 79, 10, 5, 255 ) );
				addChild( background_layer, -1 );
			}

			//
			// Sprite With Normal Texture : Antialias
			//
			{
				auto sprite = Sprite::create( "textures/step_typetype/step_typetype_dummy_01.png" );
				sprite->getTexture()->setAntiAliasTexParameters();
				sprite->setScale( 2.f );
				sprite->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.3f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( sprite );

				auto label = Label::createWithTTF( "Normal Texture\nSet Antialias", "fonts/arial.ttf", 9 );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					sprite->getPositionX()
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				addChild( label );
			}

			//
			// Sprite With Normal Texture : Alias
			//
			{
				auto sprite = Sprite::create( "textures/step_typetype/step_typetype_dummy_02.png" );
				sprite->getTexture()->setAliasTexParameters();
				sprite->setScale( 2.f );
				sprite->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( sprite );

				auto label = Label::createWithTTF( "Normal Texture\nSet Alias ", "fonts/arial.ttf", 9 );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					sprite->getPositionX()
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				addChild( label );
			}

			//
			// Sprite With PList
			//
			{
				SpriteFrameCache::getInstance()->addSpriteFramesWithFile( PLIST_Path, TEXTURE_Path );
				Director::getInstance()->getTextureCache()->getTextureForKey( TEXTURE_Path )->setAliasTexParameters();

				auto sprite = Sprite::createWithSpriteFrameName( "step_typetype_dummy_02.png" );
				sprite->setScale( 2.f );
				sprite->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.7f )
					, visibleOrigin.y + visibleSize.height * 0.5f
				) );
				addChild( sprite );

				auto label = Label::createWithTTF( "PList Texture\nSet Alias ", "fonts/arial.ttf", 9 );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					sprite->getPositionX()
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				addChild( label );
			}

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile( PLIST_Path );

			Node::onExit();
		}

		void BasicScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_typetype::RootScene::create() );
		}
		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE != keycode )
			{
				return;
			}

			if( isScheduled( schedule_selector( BasicScene::updateForExit ) ) )
			{
				return;
			}

			scheduleOnce( schedule_selector( BasicScene::updateForExit ), 0.f );
		}
	}
}
