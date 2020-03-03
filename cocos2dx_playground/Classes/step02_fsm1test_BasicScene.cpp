#include "step02_fsm1test_BasicScene.h"

#include <new>
#include <sstream>

#include "step02_RootScene.h"

#include "fsm1_iState.h"

USING_NS_CC;

namespace
{
	class TestState1 : public fsm1::CustomeState<TestState1, step02::fsm1test::BasicScene>
	{
	public:
		TestState1( step02::fsm1test::BasicScene& owner, fsm1::Machine& machine, const std::size_t index ) : CustomeState( owner, machine, index )
			, mElapsedTime( 0.f )
		{}

		void Enter() override
		{
			mElapsedTime = 0.f;

			CCLOG( "Test State 1 : Enter" );
			SuperStateT::Enter();
		}

		void Update( float dt ) override
		{
			mElapsedTime += dt;
			if( 2.f < mElapsedTime )
			{
				TransitionRequest( 0u );
				return;
			}
			SuperStateT::Update( dt );
		}

		void Exit() override
		{
			CCLOG( "Test State 1 : Exit" );
			SuperStateT::Exit();
		}

	private:
		float mElapsedTime;
	};

	class TestState2 : public fsm1::CustomeState<TestState1, step02::fsm1test::BasicScene>
	{
	public:
		TestState2( step02::fsm1test::BasicScene& owner, fsm1::Machine& machine, const std::size_t index ) : CustomeState( owner, machine, index )
			, mElapsedTime( 0.f )
		{}

		void Enter() override
		{
			mElapsedTime = 0.f;

			CCLOG( "Test State 2 : Enter" );
			SuperStateT::Enter();
		}

		void Update( float dt ) override
		{
			mElapsedTime += dt;
			if( 2.f < mElapsedTime )
			{
				TransitionRequest( 0u );
				return;
			}

			SuperStateT::Update( dt );
		}

		void Exit() override
		{
			CCLOG( "Test State 2 : Exit" );
			SuperStateT::Exit();
		}

	private:
		float mElapsedTime;
	};
}

namespace step02
{
	namespace fsm1test
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr ), mFSMMachine()
		{}

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
				ret->scheduleUpdate();
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

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 5, 29, 81, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// FSM
			//
			{
				auto& test_state_1 = mFSMMachine.AddState<TestState1>( *this, false );
				auto& test_state_2 = mFSMMachine.AddState<TestState2>( *this, true );

				test_state_1.AddTransition( test_state_2.GetIndex() );
				test_state_2.AddTransition( test_state_1.GetIndex() );
			}

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );

			mFSMMachine.Enter();
		}
		void BasicScene::update( float dt )
		{
			mFSMMachine.Update( dt );
			Scene::update( dt );
		}
		void BasicScene::onExit()
		{
			mFSMMachine.Exit();

			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			Node::onExit();
		}

		void BasicScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step02::RootScene::create() );
		}

		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( BasicScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( BasicScene::updateForExit ), 0.f );
				}
			}
		}
	}
}
