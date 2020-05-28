#include "step_mole_game_HittingNode.h"

#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "ui/UIButton.h"

#include "step_mole_AnimationComponent.h"
#include "step_mole_animation_InfoContainer.h"

USING_NS_CC;

namespace
{
	const int TAG_EffectNode = 20140416;
}

namespace step_mole
{
	namespace game
	{
		HittingNode::HittingNode( const HittingCallback& hitting_callback ) : mHittingCallback( hitting_callback ){}

		HittingNode* HittingNode::create(
			const StageConfig& stage_config
			, const HittingNodeConfig& hitting_node_config
			, const HittingCallback& hitting_callback
		)
		{
			auto ret = new ( std::nothrow ) HittingNode( hitting_callback );
			if( !ret || !ret->init( stage_config, hitting_node_config ) )
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

		bool HittingNode::init( const StageConfig& stage_config, const HittingNodeConfig& hitting_node_config )
		{
			if( !Node::init() )
			{
				return false;
			}

			const Size StageSize(
				stage_config.BlockSize.width * stage_config.BlockCount_Horizontal
				, stage_config.BlockSize.width * stage_config.BlockCount_Vercital
			);
			setContentSize( StageSize );

			// Pivot
			if( hitting_node_config.bShowPivot )
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 4.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			if( hitting_node_config.bShowNodeGuide )
			{
				auto background_guide = LayerColor::create( Color4B( 255, 0, 255, 150 ), getContentSize().width, getContentSize().height );
				addChild( background_guide, std::numeric_limits<int>::min() );
			}

			//
			// Click Area
			//
			{
				auto click_area = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				click_area->setScale9Enabled( true );
				click_area->setContentSize( StageSize );
				click_area->addTouchEventListener( CC_CALLBACK_2( HittingNode::onStageClick, this ) );
				click_area->setPosition( Vec2(
					StageSize.width * 0.5f
					, StageSize.height * 0.5f
				) );
				addChild( click_area );
			}

			//
			// Effect Node
			//
			{
				auto effect_node = Sprite::createWithSpriteFrameName( "step_mole_target_wait_0.png" );
				effect_node->setTag( TAG_EffectNode );
				effect_node->setScale( 2.f );
				effect_node->setVisible( false );
				addChild( effect_node, 1 );

				effect_node->addComponent( step_mole::AnimationComponent::create( step_mole::animation::GetObjectInfoContainer() ) );
			}

			return true;
		}

		void HittingNode::onStageClick( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			auto button = static_cast<ui::Button*>( sender );

			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			//
			// Show Effect
			//
			auto effect_node = getChildByTag( TAG_EffectNode );
			effect_node->setVisible( true );
			effect_node->setPosition( this->convertToNodeSpace( button->getTouchBeganPosition() ) );
			{
				auto animation_component = static_cast<step_mole::AnimationComponent*>( getChildByTag( TAG_EffectNode )->getComponent( step_mole::AnimationComponent::GetStaticName() ) );
				animation_component->PlayAnimationWithCallback(
					cpg::animation::eIndex::damaged_2
					, [effect_node]()
					{
						effect_node->setVisible( false );
					}
				);
			}

			//
			// Callback
			//
			mHittingCallback( static_cast<int>( button->getTouchBeganPosition().x ), static_cast<int>( button->getTouchBeganPosition().y ) );
		}
	} // namespace game
} // namespace step_mole
