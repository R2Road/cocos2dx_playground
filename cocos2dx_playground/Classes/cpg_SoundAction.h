#pragma once

#include <string>

#include "2d/CCActionInstant.h"

namespace cpg
{
	class SoundAction : public cocos2d::ActionInstant
	{
	private:
		SoundAction( const char* sound_path );

		//
		// Non-Copyable
		//
		SoundAction( const SoundAction& ) = delete;
		SoundAction( SoundAction&& ) = delete;

		//
		// Non-Assignable
		//
		SoundAction& operator =( const SoundAction& ) = delete;
		SoundAction& operator =( SoundAction&& ) = delete;

	public:
		static SoundAction* create( const char* sound_path );

	public:
		void update( float time ) override;
		SoundAction* reverse() const override;
		SoundAction* clone() const override;

	private:
		const std::string mSoundPath;
	};
}
