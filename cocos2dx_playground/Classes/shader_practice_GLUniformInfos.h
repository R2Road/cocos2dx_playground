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
			Info( const char* name, const GLenum type, const GLint location ) : Name( name ), Type( type ), Location( location ) {}

			std::string Name;
			GLenum Type;
			GLint Location;
		};

		using ContainerT = std::vector<Info>;

		void Load( const GLuint program_index );

		const ContainerT& GetContainer() const { return mContainer; }

	private:
		ContainerT mContainer;
	};

	const char* GLenum2String( const GLenum gl_enum );
}
