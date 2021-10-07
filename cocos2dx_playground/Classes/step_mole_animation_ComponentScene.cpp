#include "step_mole_animation_ComponentScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_Animation_Info.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "step_mole_AnimationComponent.h"

#include "step_mole_animation_InfoContainer.h"

USING_NS_CC;

namespace step_mole
{
	namespace animation
	{
		ComponentScene::ComponentScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mAnimationNode( nullptr )
		{}

		Scene* ComponentScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) ComponentScene( back_to_the_previous_scene_callback );
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

		bool ComponentScene::init()
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
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[A] : Play Animation - Wait";
				ss << cpg::linefeed;
				ss << "[S] : Play Animation - Wakeup";
				ss << cpg::linefeed;
				ss << "[D] : Play Animation - Idle";
				ss << cpg::linefeed;
				ss << "[F] : Play Animation - Sleep";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Q] : Play Animation With Callback";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[SpaceBar] : Stop Animation";

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
				auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Animation Node
			//
			{
				mAnimationNode = Sprite::createWithSpriteFrameName( "step_mole_target_dmgd2_4.png" );
				mAnimationNode->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				mAnimationNode->setScale( _director->getContentScaleFactor() );
				mAnimationNode->setPosition(
					visibleOrigin
					+ Vec2( static_cast<int>( visibleSize.width * 0.5f ), static_cast<int>( visibleSize.height * 0.5f ) )
				);
				addChild( mAnimationNode );


				mAnimationNode->addComponent( step_mole::AnimationComponent::create( step_mole::animation::GetObjectInfoContainer() ) );
			}

			return true;
		}

		void ComponentScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ComponentScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ComponentScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void ComponentScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_A:
				PlayAnimation( cpg_animation::eIndex::wait );
				break;

			case EventKeyboard::KeyCode::KEY_S:
				PlayAnimation( cpg_animation::eIndex::wakeup );
				break;

			case EventKeyboard::KeyCode::KEY_D:
				PlayAnimation( cpg_animation::eIndex::idle );
				break;

			case EventKeyboard::KeyCode::KEY_F:
				PlayAnimation( cpg_animation::eIndex::sleep );
				break;

			case EventKeyboard::KeyCode::KEY_Q: // Play With Callback
				PlayAnimationWithCallback();
				break;

			case EventKeyboard::KeyCode::KEY_SPACE: // Stop
				StopAnimation();
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
		void ComponentScene::PlayAnimation( const cpg_animation::eIndex animation_index )
		{
			auto animation_component = static_cast<step_mole::AnimationComponent*>( mAnimationNode->getComponent( step_mole::AnimationComponent::GetStaticName() ) );
			animation_component->PlayAnimation( animation_index );
		}
		void ComponentScene::PlayAnimationWithCallback()
		{
			auto animation_component = static_cast<step_mole::AnimationComponent*>( mAnimationNode->getComponent( step_mole::AnimationComponent::GetStaticName() ) );
			animation_component->PlayAnimationWithCallback(
				cpg_animation::eIndex::damaged_1
				, [animation_component]()
				{
					animation_component->PlayAnimationWithCallback( cpg_animation::eIndex::damaged_2, []() {} );
				}
			);
		}
		void ComponentScene::StopAnimation()
		{
			auto animation_component = static_cast<step_mole::AnimationComponent*>( mAnimationNode->getComponent( step_mole::AnimationComponent::GetStaticName() ) );
			animation_component->StopAnimation();
		}
	}
}
