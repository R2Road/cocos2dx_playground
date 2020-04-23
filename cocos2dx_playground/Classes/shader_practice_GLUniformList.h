#pragma once

#include <string>
#include <unordered_map>

#include "platform/CCGL.h"

namespace shader_practice
{
	class GLUniformList
	{
	public:
		using ContainerT = std::unordered_map<std::string, GLenum>;

		void Load( const GLuint program_index );

		const ContainerT& GetContainer() const { return mContainer; }

	private:
		ContainerT mContainer;
	};
}
