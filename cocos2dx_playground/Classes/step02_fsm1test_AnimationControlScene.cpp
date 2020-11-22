#include "step02_fsm1test_AnimationControlScene.h"

#include <new>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_StringTable.h"

#include "fsm1_iState.h"

USING_NS_CC;

const int TAG_Actor = 20140416;

namespace
{
	class IdleState : public fsm1::CustomeState<IdleState, step02::fsm1test::AnimationControlScene>
	{
	public:
		IdleState( MyOwnerT& owner, fsm1::Machine& machine, const std::size_t index ) : CustomeState( owner, machine, index )
			, mKeyCodeCollector( nullptr )
		{}

		void Setup( step_rain_of_chaos::input::KeyCodeCollector* const keycode_collector )
		{
			mKeyCodeCollector = keycode_collector;
		}

		void Update( float dt ) override
		{
			if(
				mKeyCodeCollector->isActiveKey( EventKeyboard::KeyCode::KEY_UP_ARROW )
				|| mKeyCodeCollector->isActiveKey( EventKeyboard::KeyCode::KEY_DOWN_ARROW )
				|| mKeyCodeCollector->isActiveKey( EventKeyboard::KeyCode::KEY_RIGHT_ARROW )
				|| mKeyCodeCollector->isActiveKey( EventKeyboard::KeyCode::KEY_LEFT_ARROW )
			)
			{
				TransitionRequest( 0u );
				return;
			}

			SuperStateT::Update( dt );
		}

	private:
		step_rain_of_chaos::input::KeyCodeCollector* mKeyCodeCollector;
	};

	class MoveState : public fsm1::CustomeState<MoveState, step02::fsm1test::AnimationControlScene>
	{
	public:
		MoveState( MyOwnerT& owner, fsm1::Machine& machine, const std::size_t index ) : CustomeState( owner, machine, index )
			, mKeyCodeCollector( nullptr )
			, mMoveSpeed( 3 )
		{}

		void Setup( step_rain_of_chaos::input::KeyCodeCollector* const keycode_collector )
		{
			mKeyCodeCollector = keycode_collector;
		}

		void Update( float dt ) override
		{
			Vec2 input_vec2;
			if( mKeyCodeCollector->isActiveKey( EventKeyboard::KeyCode::KEY_UP_ARROW ) )
			{
				input_vec2.y += 1.f;
			}
			if( mKeyCodeCollector->isActiveKey( EventKeyboard::KeyCode::KEY_DOWN_ARROW ) )
			{
				input_vec2.y -= 1.f;
			}
			if( mKeyCodeCollector->isActiveKey( EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) )
			{
				input_vec2.x += 1.f;
			}
			if( mKeyCodeCollector->isActiveKey( EventKeyboard::KeyCode::KEY_LEFT_ARROW ) )
			{
				input_vec2.x -= 1.f;
			}

			if( std::numeric_limits<float>::epsilon() < std::abs( input_vec2.x ) || std::numeric_limits<float>::epsilon() < std::abs( input_vec2.y ) )
			{
				//
				// Move
				//
				input_vec2.normalize();
				input_vec2.scale( mMoveSpeed );
				auto actor_root_node = mOwner.getChildByTag( TAG_Actor );
				actor_root_node->setPosition( actor_root_node->getPosition() + input_vec2 );
			}
			else
			{
				TransitionRequest( 0u );
				return;
			}

			SuperStateT::Update( dt );
		}

	private:
		step_rain_of_chaos::input::KeyCodeCollector* mKeyCodeCollector;
		const int mMoveSpeed;
	};
}

namespace step02
{
	namespace fsm1test
	{
		AnimationControlScene::AnimationControlScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* AnimationControlScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) AnimationControlScene( back_to_the_previous_scene_callback );
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
			}
			else
			{
				ret->scheduleUpdate();
				ret->autorelease();
			}

			return ret;
		}

		bool AnimationControlScene::init()
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
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow Key] : Move Actor";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}
			
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 15, 49, 101, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// Actor
			//
			{
				auto actor_root_node = Node::create();
				actor_root_node->setTag( TAG_Actor );
				actor_root_node->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				{
					// View
					auto view_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
					view_node->setScale( 2.f );
					actor_root_node->addChild( view_node );
					{
						auto animation_object = Animation::create();
						animation_object->setDelayPerUnit( 0.2f );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

						auto animate_action = Animate::create( animation_object );

						auto repeat_action = RepeatForever::create( animate_action );

						view_node->runAction( repeat_action );
					}
				}
				addChild( actor_root_node, 100 );
			}

			//
			// FSM
			//
			{
				auto& test_state_0 = mFSMMachine.AddState<IdleState>( *this, false );
				test_state_0.Setup( &mKeyCodeCollector );
				auto& test_state_1 = mFSMMachine.AddState<MoveState>( *this, true );
				test_state_1.Setup( &mKeyCodeCollector );

				test_state_0.AddTransition( test_state_1.GetIndex() );
				test_state_1.AddTransition( test_state_0.GetIndex() );
			}

			return true;
		}

		void AnimationControlScene::onEnter()
		{
			Scene::onEnter();
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( AnimationControlScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( AnimationControlScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );

			mFSMMachine.Enter();
		}
		void AnimationControlScene::update( float dt )
		{
			mFSMMachine.Update( dt );
			Scene::update( dt );
		}
		void AnimationControlScene::onExit()
		{
			mFSMMachine.Exit();

			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			Scene::onExit();
		}

		void AnimationControlScene::updateForExit( float /*dt*/ )
		{
			helper::BackToThePreviousScene::MoveBack();
		}
		void AnimationControlScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( AnimationControlScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( AnimationControlScene::updateForExit ), 0.f );
				}
				return;
			}

			mKeyCodeCollector.onKeyPressed( keycode );
		}
		void AnimationControlScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				return;
			}

			mKeyCodeCollector.onKeyReleased( keycode );
		}
	}
}
