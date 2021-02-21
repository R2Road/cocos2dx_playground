#pragma once

#include <new>
#include <string>

#include "2d/CCActionInstant.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

namespace cpg
{
	class SoundAction : public ActionInstant
	{
	private:
		SoundAction( const char* sound_path ) : mSoundPath( sound_path )
		{}

		CC_DISALLOW_COPY_AND_ASSIGN( SoundAction );

	public:
		static SoundAction* create( const char* sound_path )
		{
			auto ret = new ( std::nothrow ) SoundAction( sound_path );
			if( ret )
			{
				ret->autorelease();
			}
			else
			{
				CC_SAFE_DELETE( ret );
			}

			return ret;
		}

	public:
		void update( float time ) override
		{
			ActionInstant::update( time );
			experimental::AudioEngine::play2d( mSoundPath, false, 0.1f );
		}
		SoundAction* reverse() const override
		{
			return this->clone();
		}
		SoundAction* clone() const override
		{
			auto a = new ( std::nothrow ) SoundAction( mSoundPath.c_str() );
			a->autorelease();

			return a;
		}

	private:
		const std::string mSoundPath;
	};
}
