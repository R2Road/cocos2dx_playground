#include "step_clickclick_game_BlockView.h"

#include <new>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/ccMacros.h"
#include "ui/UIButton.h"
#include "2d/CCSpriteFrameCache.h"

#include "step_clickclick_game_Stage.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		BlockView::BlockView(
			cocos2d::Node* const button_node, cocos2d::Sprite* const view_node, cocos2d::Label* const label_node
			, cocos2d::Sprite* const effect_node
			, cocos2d::Action* const increase_effect_action
			, cocos2d::Action* const decrease_effect_action
			, cocos2d::Action* const die_effect_action
			
		) :
			mButtonNode( button_node )
			, mViewNode( view_node )
			, mLabelNode( label_node )
			, mEffectNode( effect_node )
			, mIncreaseEffectAction( increase_effect_action )
			, mDecreaseEffectAction( decrease_effect_action )
			, mDieEffectAction( die_effect_action )
		{}
		void BlockView::Reset( eBlockType type, const int life )
		{
			mLabelNode->setString( std::to_string( life ) );

			SpriteFrame* view_frame = nullptr;
			switch( type )
			{
			case eBlockType::Single:
				view_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_block_single.png" );
				break;
			case eBlockType::Same:
				view_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_block_together.png" );
				break;
			case eBlockType::Different:
				view_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_clickclick_block_different.png" );
				break;
			default:
				assert( false );
			}
			mViewNode->setSpriteFrame( view_frame );
		}
		void BlockView::SetVisible( const bool visible )
		{
			mButtonNode->setVisible( visible );
			mViewNode->setVisible( visible );
			mLabelNode->setVisible( visible );
		}
		void BlockView::UpdateLife( const int last_life, const int current_life )
		{
			if( 0 == current_life )
			{
				SetVisible( false );
				mEffectNode->stopAllActions();
				mEffectNode->runAction( mDieEffectAction );
			}
			else
			{
				mLabelNode->setString( std::to_string( current_life ) );

				if( last_life < current_life )
				{
					mEffectNode->stopAllActions();
					mEffectNode->runAction( mIncreaseEffectAction );
				}
				else
				{
					mEffectNode->stopAllActions();
					mEffectNode->runAction( mDecreaseEffectAction );
				}
			}
		}
	} // namespace game
} // namespace step_clickclick
