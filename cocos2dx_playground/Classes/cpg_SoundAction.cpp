#include "cpg_SoundAction.h"

#include <new>

#include "audio/include/AudioEngine.h"

USING_NS_CC;

namespace cpg
{
	SoundAction::SoundAction( const char* sound_path ) : mSoundPath( sound_path )
	{}

	SoundAction* SoundAction::create( const char* sound_path )
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

	void SoundAction::update( float time )
	{
		ActionInstant::update( time );
		experimental::AudioEngine::play2d( mSoundPath, false, 0.1f );
	}
	SoundAction* SoundAction::reverse() const
	{
		return this->clone();
	}
	SoundAction* SoundAction::clone() const
	{
		return create( mSoundPath.c_str() );
	}
}