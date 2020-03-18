#include "step02_animation_ListScene.h"

#include <new>
#include <sstream>

#include "cpg_Animation_Info.h"

#include "step_rain_of_chaos_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_AnimationNode = 20140416;

	const std::vector<cpg::animation::Info> AnimationInfos = {
		{
			cpg::animation::eIndex::idle
			, 0.5f
			, { "actor001_idle_01.png", "actor001_idle_02.png", "actor001_idle_03.png" }
		}
		,{
			cpg::animation::eIndex::run
			, 0.2f
			, { "actor001_run_01.png", "actor001_run_02.png", "actor001_run_03.png", "actor001_run_04.png" }
		}
		,{
			cpg::animation::eIndex::win
			, 0.1f
			, { "actor001_win_01.png", "actor001_win_02.png" }
		}
	};
}

namespace step02
{
	namespace animation
	{
		ListScene::ListScene() : mKeyboardListener( nullptr ), mAnimationActions()
		{}

		Scene* ListScene::create()
		{
			auto ret = new ( std::nothrow ) ListScene();
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

		bool ListScene::init()
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
				ss << "[A] : Play Animation - Idle";
				ss << std::endl;
				ss << "[S] : Play Animation - Run";
				ss << std::endl;
				ss << "[D] : Play Animation - Win";
				ss << std::endl;
				ss << "[SpaceBar] : Stop Animation";

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
				auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// Animation Node
			//
			{
				auto animation_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				animation_node->setTag( TAG_AnimationNode );
				animation_node->setAnchorPoint( Vec2( 0.f, 0.f ) );
				animation_node->setScale( 2.f );
				animation_node->setPosition( Vec2(
					static_cast<int>( visibleOrigin.x + ( visibleSize.width * 0.5f ) - ( animation_node->getContentSize().width * 0.5f ) )
					, static_cast<int>( visibleOrigin.y + ( visibleSize.height * 0.5f ) - ( animation_node->getContentSize().height * 0.5f ) )
				) );
				addChild( animation_node, 1 );
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

				auto repeat_action = RepeatForever::create( animate_action );
				repeat_action->setTag( static_cast<int>( animation_info.Index ) );
				repeat_action->retain();

				mAnimationActions.push_back( repeat_action );
			}

			return true;
		}

		void ListScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ListScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void ListScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			for( auto a : mAnimationActions )
			{
				a->release();
			}
			mAnimationActions.clear();

			Node::onExit();
		}

		void ListScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_rain_of_chaos::RootScene::create() );
		}

		void ListScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				if( !isScheduled( schedule_selector( ListScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( ListScene::updateForExit ), 0.f );
				}
				break;

			case EventKeyboard::KeyCode::KEY_A: // Play Idle
				playAnimation( cpg::animation::eIndex::idle );
				break;

			case EventKeyboard::KeyCode::KEY_S: // Play Run
				playAnimation( cpg::animation::eIndex::run );
				break;

			case EventKeyboard::KeyCode::KEY_D: // Play Win
				playAnimation( cpg::animation::eIndex::win );
				break;

			case EventKeyboard::KeyCode::KEY_SPACE: // Play Win
				stopAnimation();
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void ListScene::playAnimation( const cpg::animation::eIndex animation_index )
		{
			auto animation_node = getChildByTag( TAG_AnimationNode );
			assert( animation_node );

			auto animation_action = getAnimationAction( animation_index );
			if( !animation_action )
			{
				return;
			}

			animation_node->stopAllActions();
			animation_node->runAction( animation_action );
		}
		void ListScene::stopAnimation()
		{
			auto animation_node = getChildByTag( TAG_AnimationNode );
			assert( animation_node );

			animation_node->stopAllActions();
		}
		cocos2d::Action* ListScene::getAnimationAction( const cpg::animation::eIndex animation_index )
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
	}
}
