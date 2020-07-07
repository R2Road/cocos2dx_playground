#pragma once

#include "math/CCGeometry.h"
#include "math/Vec2.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class StageConfig
		{
		public:
			void Build( const int center_x, const int center_y, const int width, const int height )
			{
				mCenter.set( center_x, center_y );
				mStageArea.setRect(
					mCenter.x - ( width * 0.5f ), mCenter.y - ( height * 0.5f )
					, width, height
				);
				mBulletLifeArea.setRect(
					mStageArea.origin.x - 60.f, mStageArea.origin.y - 60.f
					, mStageArea.size.width + 60.f + 60.f, mStageArea.size.height + 60.f + 60.f
				);
				mBulletGenerateArea.setRect(
					mStageArea.origin.x - 30.f, mStageArea.origin.y - 30.f
					, mStageArea.size.width + 30.f + 30.f, mStageArea.size.height + 30.f + 30.f
				);
			}

			cocos2d::Vec2 GetCenter() const { return mCenter; }
			cocos2d::Rect GetStageArea() const { return mStageArea; }
			cocos2d::Rect GetBulletLifeArea() const { return  mBulletLifeArea; }
			cocos2d::Rect GetBulletGenerateArea() const { return mBulletGenerateArea; }

		private:
			cocos2d::Vec2 mCenter;
			cocos2d::Rect mStageArea;
			cocos2d::Rect mBulletLifeArea;
			cocos2d::Rect mBulletGenerateArea;
		};
	}
}
