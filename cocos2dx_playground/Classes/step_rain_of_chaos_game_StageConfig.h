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
			void Build( const int center_x, const int center_y, const int width )
			{
				mCenter.set( center_x, center_y );

				mStageRect.setRect(
					mCenter.x - ( width * 0.5f ), mCenter.y - ( width * 0.5f )
					, width, width
				);

				float temp_radius = sqrt( ( ( mStageRect.size.width * 0.5f ) * ( mStageRect.size.width * 0.5f ) ) * 2.f ) + 8.f;
				mBulletGenerateRadiusMin = temp_radius;

				temp_radius += 40.f;
				mBulletGenerateRadiusMax = temp_radius;

				temp_radius += 20.f;
				mBulletLifeRect.setRect(
					mCenter.x - temp_radius, mCenter.y - temp_radius
					, temp_radius * 2.f, temp_radius * 2.f
				);
			}

			cocos2d::Vec2 GetCenter() const { return mCenter; }

			cocos2d::Rect GetStageRect() const { return mStageRect; }
			cocos2d::Rect GetBulletLifeRect() const { return  mBulletLifeRect; }

			float GetBulletGenerateRadiusMin() const { return mBulletGenerateRadiusMin; }
			float GetBulletGenerateRadiusMax() const { return mBulletGenerateRadiusMax; }

		private:
			cocos2d::Vec2 mCenter;

			cocos2d::Rect mStageRect;
			cocos2d::Rect mBulletLifeRect;

			float mBulletGenerateRadiusMin;
			float mBulletGenerateRadiusMax;
		};
	}
}
