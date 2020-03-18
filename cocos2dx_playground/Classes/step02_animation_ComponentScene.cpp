#include "step02_animation_ComponentScene.h"

#include <new>
#include <sstream>

#include "cpg_Animation_Info.h"
#include "cpg_AnimationComponent.h"
#include "cpg_animation_InfoContainer.h"
#include "step_rain_of_chaos_RootScene.h"

USING_NS_CC;

const int TAG_AnimationNode = 20140416;

namespace step02
{
	namespace animation
	{
		ComponentScene::ComponentScene() : mKeyboardListener( nullptr )
		{}

		Scene* ComponentScene::create()
		{
			auto ret = new ( std::nothrow ) ComponentScene();
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

		bool ComponentScene::init()
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


				const auto animation_info_container = cpg::animation::InfoContainer::create();
				animation_node->addComponent( cpg::AnimationComponent::create( *animation_info_container ) );
			}

			return true;
		}

		void ComponentScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ComponentScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void ComponentScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void ComponentScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_rain_of_chaos::RootScene::create() );
		}

		void ComponentScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				if( !isScheduled( schedule_selector( ComponentScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( ComponentScene::updateForExit ), 0.f );
				}
				break;

			case EventKeyboard::KeyCode::KEY_A: // Play Idle
				PlayAnimation( cpg::animation::eIndex::idle );
				break;

			case EventKeyboard::KeyCode::KEY_S: // Play Run
				PlayAnimation( cpg::animation::eIndex::run );
				break;

			case EventKeyboard::KeyCode::KEY_D: // Play Win
				PlayAnimation( cpg::animation::eIndex::win );
				break;

			case EventKeyboard::KeyCode::KEY_SPACE: // Play Win
				StopAnimation();
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
		void ComponentScene::PlayAnimation( const cpg::animation::eIndex animation_index )
		{
			auto animation_component = static_cast<cpg::AnimationComponent*>( getChildByTag( TAG_AnimationNode )->getComponent( cpg::AnimationComponent::GetStaticName() ) );
			animation_component->PlayAnimation( animation_index );
		}
		void ComponentScene::StopAnimation()
		{
			auto animation_component = static_cast<cpg::AnimationComponent*>( getChildByTag( TAG_AnimationNode )->getComponent( cpg::AnimationComponent::GetStaticName() ) );
			animation_component->StopAnimation();
		}
	}
}
