#include "step_rain_of_chaos_game_SpawnProcessor_01_Circle_OutToIn.h"

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace
{
	const float LimitTime = 5.f;
	const int RequiredBulletCount = 100;
	const float TotalAngle = 720.f;
	const float RadianPerBullet = CC_DEGREES_TO_RADIANS( TotalAngle / RequiredBulletCount );
	const float BulletPerSeconds = LimitTime / RequiredBulletCount;
}

namespace step_rain_of_chaos
{
	namespace game
	{
		SpawnProcessor_Circle_01_OutToIn::SpawnProcessor_Circle_01_OutToIn( const StageConfig& stage_config ) : iSpawnProcessor( stage_config )
			, mRemainTime( 0.f )
			, mCurrentStartPosition( Vec2::UNIT_Y )
			, mCurrentFireCount( 0 )
		{}

		SpawnProcessorUp SpawnProcessor_Circle_01_OutToIn::Create( const StageConfig& stage_config )
		{
			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_Circle_01_OutToIn( stage_config ) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_Circle_01_OutToIn::init()
		{
			mCurrentStartPosition.scale( mStageConfig.GetBulletGenerateArea().size.width * 0.5f );
			mCurrentStartPosition += mStageConfig.GetCenter();
		}
		bool SpawnProcessor_Circle_01_OutToIn::Update( float dt, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container )
		{
			mRemainTime += dt;

			while( BulletPerSeconds <= mRemainTime )
			{
				mCurrentStartPosition.rotate( mStageConfig.GetCenter(), RadianPerBullet );

				out_spawn_info_container->push_back( SpawnInfo{
					mCurrentStartPosition
					, target_position - mCurrentStartPosition
				} );

				++mCurrentFireCount;
				if( RequiredBulletCount <= mCurrentFireCount )
				{
					break;
				}

				mRemainTime -= BulletPerSeconds;
			}

			return RequiredBulletCount > mCurrentFireCount;
		}
	}
}
