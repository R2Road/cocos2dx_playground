#include "step_flipflip_action_BasicScene.h"

#include <new>
#include <numeric>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"

#include "step_flipflip_game_Constant.h"
#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace action
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr ) {}

		Scene* BasicScene::create()
		{
			auto ret = new ( std::nothrow ) BasicScene();
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

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 29, 96, 96, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Action : Scale
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( GetSpriteFrameName_CardFrontSide( game::eCardType::A ) );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.25f, visibleSize.height * 0.5f )
				);
				addChild( sprite );

				// Action
				{
					auto scale_to_1 = ScaleTo::create( 0.5f, -1.f, 1.f );
					auto delay_time_1 = DelayTime::create( 0.5f );
					auto scale_to_2 = ScaleTo::create( 0.5f, 1.f, 1.f );
					auto delay_time_2 = DelayTime::create( 0.5f );

					auto sequence = Sequence::create( scale_to_1, delay_time_1, scale_to_2, delay_time_2, nullptr );

					auto repeat_forever = RepeatForever::create( sequence );

					sprite->runAction( repeat_forever );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Scale", "fonts/NanumSquareR.ttf", 12 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						sprite->getPosition()
						- Vec2( 0.f, 50.f )
					);
					addChild( label );
				}
			}

			//
			// Action : Fade
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( GetSpriteFrameName_CardFrontSide( game::eCardType::B ) );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);

				addChild( sprite );

				// Action
				{
					auto fade_out = FadeOut::create( 0.8f );
					auto fade_in = FadeIn::create( 0.6f );

					auto sequence = Sequence::create( fade_out, fade_in, nullptr );

					auto repeat_forever = RepeatForever::create( sequence );

					sprite->runAction( repeat_forever );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Fade In/Out", "fonts/NanumSquareR.ttf", 12 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						sprite->getPosition()
						- Vec2( 0.f, 50.f )
					);
					addChild( label );
				}
			}

			//
			// Action : Animation
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( GetSpriteFrameName_CardFrontSide( game::eCardType::A ) );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.75f, visibleSize.height * 0.5f )
				);
				addChild( sprite );

				// Action
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_flipflip_card_back_0.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_flipflip_card_front_0.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_flipflip_card_front_1.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_flipflip_card_front_2.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_flipflip_card_front_3.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_flipflip_card_front_4.png" ) );

					auto animate = Animate::create( animation_object );

					auto repeat_forever = RepeatForever::create( animate );

					sprite->runAction( repeat_forever );
				}

				// Explain
				{
					auto label = Label::createWithTTF( "Animation", "fonts/NanumSquareR.ttf", 12 );
					label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition(
						sprite->getPosition()
						- Vec2( 0.f, 50.f )
					);
					addChild( label );
				}
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


		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_flipflip::RootScene::create() );
				return;
			}
		}
	}
}
