#include "step_flipflip_game_TitleScene.h"

#include <new>
#include <numeric>
#include <vector>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"

#include "step_flipflip_game_PlayScene.h"
#include "step_flipflip_game_ResultScene.h"
#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace
{
	const std::vector<std::pair<char*, char*>> Title_Word_Sprite_Frames = {
		{ "step_flipflip_title_card_0.png", "step_flipflip_card_front_0.png" }
		, { "step_flipflip_title_card_1.png", "step_flipflip_card_front_1.png" }
		, { "step_flipflip_title_card_2.png", "step_flipflip_card_front_2.png" }
		, { "step_flipflip_title_card_3.png", "step_flipflip_card_front_3.png" }
		, { "step_flipflip_title_card_0.png", "step_flipflip_card_front_4.png" }
		, { "step_flipflip_title_card_1.png", "step_flipflip_card_front_5.png" }
		, { "step_flipflip_title_card_2.png", "step_flipflip_card_front_0.png" }
		, { "step_flipflip_title_card_3.png", "step_flipflip_card_front_1.png" }
	};
}

namespace step_flipflip
{
	namespace game
	{
		TitleScene::TitleScene() : mKeyboardListener( nullptr ) {}

		Scene* TitleScene::create()
		{
			auto ret = new ( std::nothrow ) TitleScene();
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

		bool TitleScene::init()
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
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << "[F1] : " << ResultScene::getTitle();

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 6 );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( 0.f, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}


			//
			// Title Background
			//
			{
				auto sprite = Sprite::create( "textures/step_flipflip/step_flipflip_title.png" );
				sprite->getTexture()->setAliasTexParameters();
				sprite->setScaleX( visibleSize.width / sprite->getContentSize().width );
				sprite->setScaleY( visibleSize.height / sprite->getContentSize().height );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( sprite, std::numeric_limits<int>::min() );
			}


			//
			// Title Words
			//
			{
				const Vec2 pivotPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.54f )
				);

				const float WordWidth = 42.f;
				const float WordSpacing = 2.f;
				const float WordBlockWidth = WordWidth + WordSpacing;

				const float WordsWidth = ( WordBlockWidth ) * 3;
				const float WordsMargin = 20.f;


				int i = 0;
				for( const auto& w : Title_Word_Sprite_Frames )
				{
					auto sprite = Sprite::createWithSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( w.first ) );
					if( 4 > i )
					{
						sprite->setPosition(
							pivotPosition
							+ Vec2( -WordsMargin, 20.f )
							+ Vec2( -WordsWidth, 20.f )
							+ Vec2( i * WordBlockWidth, 0.f )
						);
					}
					else
					{
						sprite->setPosition(
							pivotPosition
							+ Vec2( WordsMargin, -20.f )
							+ Vec2( ( i - 4 ) * WordBlockWidth, 0.f )
						);
					}
					addChild( sprite );

					//
					// Action
					//
					{
						// Part 1
						auto delay_time_1 = DelayTime::create( 3.f );

						auto scale_to_1 = ScaleTo::create( 0.1f, 0.f, 1.f );

						auto animation_object_1 = Animation::create();
						animation_object_1->setDelayPerUnit( 0.01f );
						animation_object_1->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( w.second ) );
						auto animate_1 = Animate::create( animation_object_1 );

						auto scale_to_2 = ScaleTo::create( 0.1f, 1.f, 1.f );

						// Part 2
						auto delay_time_2 = DelayTime::create( 0.6f );

						auto scale_to_3 = ScaleTo::create( 0.1f, 0.f, 1.f );

						auto animation_object_2 = Animation::create();
						animation_object_2->setDelayPerUnit( 0.01f );
						animation_object_2->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( w.first ) );
						auto animate_2 = Animate::create( animation_object_2 );

						auto scale_to_4 = ScaleTo::create( 0.1f, 1.f, 1.f );

						// Run
						auto sequence = Sequence::create(
							delay_time_1, scale_to_1, animate_1, scale_to_2
							, delay_time_2, scale_to_3, animate_2, scale_to_4
							, nullptr
						);
						auto repeat_forever = RepeatForever::create( sequence );
						sprite->runAction( repeat_forever );
					}

					++i;
				}
			}


			//
			// request input
			//
			{
				auto label = Label::createWithTTF( "PRESS SPACE BAR", "fonts/NanumSquareR.ttf", 14 );
				label->setPosition(
					visibleOrigin
					+  Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.23f )
				);
				addChild( label, std::numeric_limits<int>::max() );

				auto fadeOutAction = FadeOut::create( 0.8f );
				auto fadeOutkDelay = DelayTime::create( 0.2f );
				auto fadeInAction = FadeIn::create( 0.6f );
				auto fadeInkDelay = DelayTime::create( 0.4f );
				auto blinkSequence = Sequence::create( fadeOutAction, fadeOutkDelay, fadeInAction, fadeInkDelay, nullptr );
				auto blinkrepeat = RepeatForever::create( blinkSequence );
				label->runAction( blinkrepeat );
			}

			return true;
		}

		void TitleScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TitleScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void TitleScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void TitleScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_flipflip::RootScene::create() );
				return;
			}

			if( EventKeyboard::KeyCode::KEY_F1 == keycode )
			{
				_director->replaceScene( ResultScene::create( 123.456f, 300 ) );
				return;
			}

			if( EventKeyboard::KeyCode::KEY_SPACE == keycode )
			{
				_director->replaceScene( PlayScene::create() );
				return;
			}
		}
	}
}
