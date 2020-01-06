#pragma once

#include <string>

namespace research
{
	class Setting
	{
	private:
		Setting();

	public:
		static void load();

		static const std::string& getKeyAllowFileName() { return instance.fileName_for_keyAllow; }
		static const std::string& getKeyMapFileName() { return instance.fileName_for_keyMap; }

	private:
		static Setting instance;

		std::string fileName_for_keyAllow;
		std::string fileName_for_keyMap;
	};
}