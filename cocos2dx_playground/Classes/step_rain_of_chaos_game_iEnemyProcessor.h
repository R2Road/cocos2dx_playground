#pragma once

#include <memory>

#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN
class Node;
NS_CC_END

//
// # Needs
// 1. Move
// 2. Fire
// 3. Move with Fire
//

namespace step_rain_of_chaos
{
	namespace game
	{
		class StageConfig;

		using EnemyProcessorUp = std::unique_ptr<class iEnemyProcessor>;

		class iEnemyProcessor
		{
		protected:
			iEnemyProcessor( const StageConfig& stage_config, cocos2d::Node* const owner_node, cocos2d::Node* const target_node ) :
				mStageConfig( stage_config )
				, mOwnerNode( owner_node )
				, mTargetNode( target_node )
			{}

		public:
			virtual ~iEnemyProcessor() {}

			virtual void init() {}
			virtual void Enter() {}
			virtual bool Update( const float delta_time ) = 0;

		protected:
			const StageConfig& mStageConfig;
			cocos2d::Node* const mOwnerNode;
			cocos2d::Node* const mTargetNode;
		};
	}
}
