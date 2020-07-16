#include "step_rain_of_chaos_game_test_ActorMoveScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/ccUTF8.h"

#include "step_mole_AnimationComponent.h"
#include "step_rain_of_chaos_game_AnimationInfoContainer.h"

USING_NS_CC;

namespace
{
	const int TAG_AnimationNode = 20140416;
	const int TAG_MoveSpeedNode = 20160528;
}

namespace step_rain_of_chaos
{
	namespace game_test
	{
		ActorMoveScene::ActorMoveScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mKeyCodeCollector()
			, mMoveSpeed( 3.f )
		{}

		Scene* ActorMoveScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) ActorMoveScene( back_to_the_previous_scene_callback );
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

		bool ActorMoveScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			scheduleUpdate();

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
				ss << std::endl;
				ss << std::endl;
				ss << "[1] : Move Speed Up";
				ss << std::endl;
				ss << "[2] : Move Speed Down";
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow Key] : Move";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 130, 49, 29, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Current Life Time
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
				label->setTag( TAG_MoveSpeedNode );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateMoveSpeedView();
			}

			//
			// Animation
			//
			{
				auto animation_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				animation_node->setTag( TAG_AnimationNode );
				animation_node->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				animation_node->setScale( _director->getContentScaleFactor() );
				animation_node->setPosition( Vec2(
					static_cast<int>( visibleOrigin.x + ( visibleSize.width * 0.5f ) )
					, static_cast<int>( visibleOrigin.y + ( visibleSize.height * 0.5f ) )
				) );
				addChild( animation_node );
				{
					// Animation Component
					auto animation_component = step_mole::AnimationComponent::create( step_rain_of_chaos::game::GetActorAnimationInfoContainer() );
					animation_node->addComponent( animation_component );

					animation_component->PlayAnimation( cpg::animation::eIndex::run );
				}

			}

			return true;
		}

		void ActorMoveScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ActorMoveScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( ActorMoveScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ActorMoveScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void ActorMoveScene::update( float dt )
		{
			Vec2 move_vector;
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_UP_ARROW ) )
			{
				move_vector.y += 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_DOWN_ARROW ) )
			{
				move_vector.y -= 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) )
			{
				move_vector.x += 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_LEFT_ARROW ) )
			{
				move_vector.x -= 1.f;
			}

			if( 0.f != move_vector.x || 0.f != move_vector.y )
			{
				move_vector.normalize();
				move_vector.scale( mMoveSpeed );

				auto animation_node = getChildByTag( TAG_AnimationNode );
				animation_node->setPosition( animation_node->getPosition() + move_vector );

				updateMoveSpeedView();
			}

			Scene::update( dt );
		}


		void ActorMoveScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}

			if( EventKeyboard::KeyCode::KEY_1 == keycode )
			{
				mMoveSpeed += 1.f;
				updateMoveSpeedView();
			}
			if( EventKeyboard::KeyCode::KEY_2 == keycode )
			{
				mMoveSpeed = std::max( 1.f, mMoveSpeed - 1.f );
				updateMoveSpeedView();
			}

			mKeyCodeCollector.onKeyPressed( keycode );
		}
		void ActorMoveScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			mKeyCodeCollector.onKeyReleased( keycode );
		}

		void ActorMoveScene::updateMoveSpeedView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_MoveSpeedNode ) );
			label->setString( StringUtils::format( "Move Speed : %.2f", mMoveSpeed ) );
		}
	}
}
