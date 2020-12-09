#pragma once

#include <string>

namespace input_practice
{
	class Setting
	{
	private:
		Setting();

	public:
		static void load();

		static const std::string& getKeyAllowFileName() { return instance.mFileName_for_KeyAllow; }
		static const std::string& getKeyMapFileName() { return instance.mFileName_for_KeyMap; }

	private:
		static Setting instance;

		std::string mFileName_for_KeyAllow;
		std::string mFileName_for_KeyMap;
	};
}