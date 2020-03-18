#include "step02_fsm1test_BasicScene.h"

#include <new>
#include <sstream>

#include "step_rain_of_chaos_RootScene.h"

#include "fsm1_iState.h"

USING_NS_CC;

namespace
{
	const int TAG_Label_State_0 = 20140416;
	const int TAG_Label_State_1 = 20160528;

	void UpdateStateStatusView( Label* label, const int state_index, const float elapsed_time, const float limit_time )
	{
		label->setString( StringUtils::format( "State %d : %.2f / %.2f", state_index, elapsed_time, limit_time ) );
	}

	class TestState1 : public fsm1::CustomeState<TestState1, step02::fsm1test::BasicScene>
	{
	public:
		TestState1( MyOwnerT& owner, fsm1::Machine& machine, const std::size_t index ) : CustomeState( owner, machine, index )
			, mLimitTime( 2.f )
			, mElapsedTime( 0.f )
			, mTargetLabelTag( -1 )
		{}

		void Setup( const int target_label_tag )
		{
			mTargetLabelTag = target_label_tag;

			auto label = static_cast<Label*>( mOwner.getChildByTag( mTargetLabelTag ) );
			label->setColor( Color3B::GRAY );
			UpdateStateStatusView( label, GetIndex(), 0.f, mLimitTime );
		}

		void Enter() override
		{
			mElapsedTime = 0.f;

			mOwner.getChildByTag( mTargetLabelTag )->setColor( Color3B::RED );

			CCLOG( "State %d : Enter", GetIndex() );
			SuperStateT::Enter();
		}

		void Update( float dt ) override
		{
			mElapsedTime += dt;
			if( mLimitTime < mElapsedTime )
			{
				TransitionRequest( 0u );
				return;
			}
			else
			{
				auto label = static_cast<Label*>( mOwner.getChildByTag( mTargetLabelTag ) );
				UpdateStateStatusView( label, GetIndex(), mElapsedTime, mLimitTime );
			}
			SuperStateT::Update( dt );
		}

		void Exit() override
		{
			SuperStateT::Exit();
			CCLOG( "State %d : Exit", GetIndex() );

			auto label = static_cast<Label*>( mOwner.getChildByTag( mTargetLabelTag ) );
			label->setColor( Color3B::GRAY );
			UpdateStateStatusView( label, GetIndex(), 0.f, mLimitTime );
		}

	private:
		const float mLimitTime;
		float mElapsedTime;
		int mTargetLabelTag;
	};

	class TestState2 : public fsm1::CustomeState<TestState1, step02::fsm1test::BasicScene>
	{
	public:
		TestState2( MyOwnerT& owner, fsm1::Machine& machine, const std::size_t index ) : CustomeState( owner, machine, index )
			, mLimitTime( 3.f )
			, mElapsedTime( 0.f )
			, mTargetLabelTag( -1 )
		{}

		void Setup( const int target_label_tag )
		{
			mTargetLabelTag = target_label_tag;

			auto label = static_cast<Label*>( mOwner.getChildByTag( mTargetLabelTag ) );
			label->setColor( Color3B::GRAY );
			UpdateStateStatusView( label, GetIndex(), 0.f, mLimitTime );
		}

		void Enter() override
		{
			mElapsedTime = 0.f;

			mOwner.getChildByTag( mTargetLabelTag )->setColor( Color3B::RED );

			CCLOG( "State %d : Enter", GetIndex() );
			SuperStateT::Enter();
		}

		void Update( float dt ) override
		{
			mElapsedTime += dt;
			if( mLimitTime < mElapsedTime )
			{
				TransitionRequest( 0u );
				return;
			}
			else
			{
				auto label = static_cast<Label*>( mOwner.getChildByTag( mTargetLabelTag ) );
				UpdateStateStatusView( label, GetIndex(), mElapsedTime, mLimitTime );

				static const float r = ( Color3B::GRAY.r - Color3B::RED.r );
				static const float g = ( Color3B::GRAY.g - Color3B::RED.g );
				static const float b = ( Color3B::GRAY.b - Color3B::RED.b );
				const float color_modify_rate = mElapsedTime / mLimitTime;

				label->setColor( Color3B(
					Color3B::RED.r + static_cast<GLubyte>( r * color_modify_rate )
					, Color3B::RED.g + static_cast<GLubyte>( g * color_modify_rate )
					, Color3B::RED.b + static_cast<GLubyte>( b * color_modify_rate )
				) );
			}

			SuperStateT::Update( dt );
		}

		void Exit() override
		{
			
			SuperStateT::Exit();
			CCLOG( "State %d : Exit", GetIndex() );

			auto label = static_cast<Label*>( mOwner.getChildByTag( mTargetLabelTag ) );
			label->setColor( Color3B::GRAY );
			UpdateStateStatusView( label, GetIndex(), 0.f, mLimitTime );
		}

	private:
		const float mLimitTime;
		float mElapsedTime;
		int mTargetLabelTag;
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
				ss << std::endl;
				ss << std::endl;
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
			// View
			//
			{
				{
					auto label = Label::createWithTTF( "State 1", "fonts/arial.ttf", 12 );
					label->setTag( TAG_Label_State_0 );
					label->setColor( Color3B::GRAY );
					label->setPosition( Vec2(
						visibleOrigin.x + ( visibleSize.width * 0.5f )
						, visibleOrigin.y + ( visibleSize.height * 0.7f )
					) );
					addChild( label, 1 );
				}

				{
					auto label = Label::createWithTTF( "State 2", "fonts/arial.ttf", 12 );
					label->setColor( Color3B::GRAY );
					label->setTag( TAG_Label_State_1 );
					label->setPosition( Vec2(
						visibleOrigin.x + ( visibleSize.width * 0.5f )
						, visibleOrigin.y + ( visibleSize.height * 0.3f )
					) );
					addChild( label, 1 );
				}
			}

			//
			// FSM
			//
			{
				auto& test_state_0 = mFSMMachine.AddState<TestState1>( *this, false );
				test_state_0.Setup( TAG_Label_State_0 );
				auto& test_state_1 = mFSMMachine.AddState<TestState2>( *this, true );
				test_state_1.Setup( TAG_Label_State_1 );

				test_state_0.AddTransition( test_state_1.GetIndex() );
				test_state_1.AddTransition( test_state_0.GetIndex() );
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
			Director::getInstance()->replaceScene( step_rain_of_chaos::RootScene::create() );
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
