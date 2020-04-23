#pragma once

#include <string>
#include <vector>

#include "platform/CCGL.h"

namespace shader_practice
{
	class GLUniformInfos
	{
	public:
		struct Info
		{
			Info( const char* name, GLenum type ) : Name( name ), Type( type ) {}

			std::string Name;
			GLenum Type;
		};

		using ContainerT = std::vector<Info>;

		void Load( const GLuint program_index );

		const ContainerT& GetContainer() const { return mContainer; }

	private:
		ContainerT mContainer;
	};
}
