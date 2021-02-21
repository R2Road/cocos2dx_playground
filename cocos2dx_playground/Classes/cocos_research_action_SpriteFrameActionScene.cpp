#include "cocos_research_action_SpriteFrameActionScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_StringTable.h"

USING_NS_CC;

namespace
{
	const int TAG_Action_Animation = 20140416;

	class SpriteFrameAction : public ActionInterval
	{
	public:
		SpriteFrameAction( SpriteFrame* sprite_frame ) : mSpriteFrame( sprite_frame ), mbFirstFrame( false ) {}

	private:
		CC_DISALLOW_COPY_AND_ASSIGN( SpriteFrameAction );

	public:
		static SpriteFrameAction* create( float duration, SpriteFrame* sprite_frame )
		{
			auto ret = new ( std::nothrow ) SpriteFrameAction( sprite_frame );

			if( ret && ret->initWithDuration( duration ) )
			{
				ret->autorelease();
				return ret;
			}

			delete ret;
			return nullptr;
		}

		//
		// Overrides
		//
		SpriteFrameAction* clone() const override
		{
			// no copy constructor
			return SpriteFrameAction::create( _duration, mSpriteFrame );
		}
		SpriteFrameAction* reverse() const  override
		{
			CCASSERT( false, "reverse() not supported in SpriteFrameAction" );
			return nullptr;
		}
		void startWithTarget( Node *target ) override
		{
			ActionInterval::startWithTarget( target );
			mbFirstFrame = true;
		}
		void update( float time ) override
		{
			if( mbFirstFrame )
			{
				mbFirstFrame = false;

				auto blend = static_cast<Sprite*>( _target )->getBlendFunc();
				static_cast<Sprite*>( _target )->setSpriteFrame( mSpriteFrame );
				static_cast<Sprite*>( _target )->setBlendFunc( blend );

				CCLOG( "SpriteFrameAction : Do" );
			}
		}

	private:
		SpriteFrame* mSpriteFrame;
		bool mbFirstFrame;
	};
}

namespace cocos_research_action
{
	SpriteFrameActionScene::SpriteFrameActionScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mTestNode( nullptr )
		, mTestAction( nullptr )
	{}

	SpriteFrameActionScene::~SpriteFrameActionScene()
	{
		mTestAction->release();
	}

	Scene* SpriteFrameActionScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) SpriteFrameActionScene( back_to_the_previous_scene_callback );
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

	bool SpriteFrameActionScene::init()
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
			ss << "[Q] : Play";
			ss << std::endl;
			ss << "[W] : Stop";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 8, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 5, 29, 81, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// Research
		//
		{
			// Node
			{
				mTestNode = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				mTestNode->setScale( _director->getContentScaleFactor() );
				mTestNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mTestNode );
			}

			// Action
			{
				Sequence* phase_1 = nullptr;
				{
					auto sprite_frame_action_1 = SpriteFrameAction::create( 0.1f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					auto sprite_frame_action_2 = SpriteFrameAction::create( 0.3f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					auto sprite_frame_action_3 = SpriteFrameAction::create( 0.1f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					auto sprite_frame_action_4 = SpriteFrameAction::create( 0.3f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );
					auto sprite_frame_action_5 = SpriteFrameAction::create( 0.1f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					auto sprite_frame_action_6 = SpriteFrameAction::create( 0.3f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					auto sprite_frame_action_7 = SpriteFrameAction::create( 0.1f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					auto sprite_frame_action_8 = SpriteFrameAction::create( 0.3f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

					phase_1 = Sequence::create(
						sprite_frame_action_1
						, sprite_frame_action_2
						, sprite_frame_action_3
						, sprite_frame_action_4
						, sprite_frame_action_5
						, sprite_frame_action_6
						, sprite_frame_action_7
						, sprite_frame_action_8
						, nullptr
					);
				}

				Sequence* phase_2 = nullptr;
				{
					auto sprite_frame_action_1 = SpriteFrameAction::create( 0.2f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					auto sprite_frame_action_2 = SpriteFrameAction::create( 0.5f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					auto sprite_frame_action_3 = SpriteFrameAction::create( 0.2f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					auto sprite_frame_action_4 = SpriteFrameAction::create( 0.5f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );
					auto sprite_frame_action_5 = SpriteFrameAction::create( 0.2f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					auto sprite_frame_action_6 = SpriteFrameAction::create( 0.5f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					auto sprite_frame_action_7 = SpriteFrameAction::create( 0.2f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					auto sprite_frame_action_8 = SpriteFrameAction::create( 0.5f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

					phase_2 = Sequence::create(
						sprite_frame_action_1
						, sprite_frame_action_2
						, sprite_frame_action_3
						, sprite_frame_action_4
						, sprite_frame_action_5
						, sprite_frame_action_6
						, sprite_frame_action_7
						, sprite_frame_action_8
						, nullptr
					);
				}

				Sequence* phase_3 = nullptr;
				{
					auto sprite_frame_action_1 = SpriteFrameAction::create( 0.1f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					auto sprite_frame_action_2 = SpriteFrameAction::create( 0.8f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					auto sprite_frame_action_3 = SpriteFrameAction::create( 0.1f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					auto sprite_frame_action_4 = SpriteFrameAction::create( 0.8f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );
					auto sprite_frame_action_5 = SpriteFrameAction::create( 0.1f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					auto sprite_frame_action_6 = SpriteFrameAction::create( 0.8f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					auto sprite_frame_action_7 = SpriteFrameAction::create( 0.1f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					auto sprite_frame_action_8 = SpriteFrameAction::create( 0.8f, SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

					phase_3 = Sequence::create(
						sprite_frame_action_1
						, sprite_frame_action_2
						, sprite_frame_action_3
						, sprite_frame_action_4
						, sprite_frame_action_5
						, sprite_frame_action_6
						, sprite_frame_action_7
						, sprite_frame_action_8
						, nullptr
					);
				}

				auto sequence_action = Sequence::create(
					phase_1
					, phase_2
					, phase_3
					, nullptr
				);				

				mTestAction = RepeatForever::create( sequence_action );
				mTestAction->setTag( TAG_Action_Animation );
				mTestAction->retain();
			}
		}
		

		return true;
	}

	void SpriteFrameActionScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SpriteFrameActionScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void SpriteFrameActionScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void SpriteFrameActionScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_Q: // Play Once
			if( !mTestNode->getActionByTag( TAG_Action_Animation ) )
			{
				mTestNode->runAction( mTestAction );
			}
			break;
		case EventKeyboard::KeyCode::KEY_W: // Stop
			if( 0 < mTestNode->getNumberOfRunningActions() )
			{
				mTestNode->stopAllActions();
			}
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
}
