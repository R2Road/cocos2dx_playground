#include "step_clickclick_animation_ListScene.h"

#include <new>
#include <numeric>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_Animation_Info.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_clickclick_RootScene.h"

USING_NS_CC;

namespace
{
	const std::vector<cpg_animation::Info> AnimationInfos = {
		{
			cpg_animation::eIndex::idle
			, 0.06f
			, std::vector<std::string>{ "actor001_idle_01.png", "actor001_idle_02.png", "actor001_idle_01.png", "actor001_idle_03.png" }
		}

		, {
			cpg_animation::eIndex::run
			, 0.1f
			, std::vector<std::string>{ "actor001_run_01.png", "actor001_run_02.png", "actor001_run_03.png", "actor001_run_04.png" }
		}

		, {
			cpg_animation::eIndex::win
			, 0.1f
			, std::vector<std::string>{ "actor001_win_01.png", "actor001_win_02.png" }
		}
	};
}

namespace step_clickclick
{
	namespace animation
	{
		ListScene::ListScene() :
			mKeyboardListener( nullptr )

			, mAnimationNode( nullptr )
			, mAnimationActions()
		{}

		ListScene::~ListScene()
		{
			for( auto a : mAnimationActions )
			{
				a->release();
			}
			mAnimationActions.clear();
		}

		Scene* ListScene::create()
		{
			auto ret = new ( std::nothrow ) ListScene();
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

		bool ListScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();
			const Vec2 visibleCenter(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			);

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[A] : Play Animation - Idle";
				ss << cpg::linefeed;
				ss << "[S] : Play Animation - Run";
				ss << cpg::linefeed;
				ss << "[D] : Play Animation - Win";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 0, 55, 25, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Animation Node
			//
			{
				mAnimationNode = Sprite::createWithSpriteFrameName( "actor001_win_01.png" );
				mAnimationNode->setScale( _director->getContentScaleFactor() );
				mAnimationNode->setPosition( visibleCenter );
				addChild( mAnimationNode );
			}

			//
			// Animation
			//
			mAnimationActions.reserve( AnimationInfos.size() );
			for( const auto& animation_info : AnimationInfos )
			{
				auto animation_object = Animation::create();
				animation_object->setDelayPerUnit( animation_info.delay );

				for( const auto& sprite_frame_name : animation_info.SpriteFrameNames )
				{
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( sprite_frame_name ) );
				}

				auto animate_action = Animate::create( animation_object );
				animate_action->setTag( static_cast<int>( animation_info.Index ) );
				animate_action->retain();

				mAnimationActions.push_back( animate_action );
			}

			return true;
		}

		void ListScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ListScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ListScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}
		

		void ListScene::playAnimation( const cpg_animation::eIndex animation_index )
		{
			auto animation_action = getAnimationAction( animation_index );
			if( !animation_action )
			{
				return;
			}

			mAnimationNode->stopAllActions();
			mAnimationNode->runAction( animation_action );
		}
		cocos2d::Action* ListScene::getAnimationAction( const cpg_animation::eIndex animation_index ) const
		{
			for( auto a : mAnimationActions )
			{
				if( static_cast<int>( animation_index ) == a->getTag() )
				{
					return a;
				}
			}

			return nullptr;
		}


		void ListScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_clickclick::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_A:
				playAnimation( cpg_animation::eIndex::idle );
				break;

			case EventKeyboard::KeyCode::KEY_S:
				playAnimation( cpg_animation::eIndex::run );
				break;

			case EventKeyboard::KeyCode::KEY_D:
				playAnimation( cpg_animation::eIndex::win );
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
