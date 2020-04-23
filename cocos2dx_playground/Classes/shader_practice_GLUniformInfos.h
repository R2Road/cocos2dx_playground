#pragma once

#include <string>
#include <vector>

#include "platform/CCGL.h"

namespace shader_practice
{
	class GLUniformList
	{
	public:
		struct GLUniformInfo
		{
			GLUniformInfo( const char* name, GLenum type ) : Name( name ), Type( type ) {}

			std::string Name;
			GLenum Type;
		};

		using ContainerT = std::vector<GLUniformInfo>;

		void Load( const GLuint program_index );

		const ContainerT& GetContainer() const { return mContainer; }

	private:
		ContainerT mContainer;
	};
}
