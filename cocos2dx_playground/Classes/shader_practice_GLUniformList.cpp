#include "shader_practice_GLUniformList.h"

#include <cassert>

#include "base/CCConsole.h"
#include "platform/CCPlatformMacros.h"

namespace shader_practice
{
	//
	// This Code is...
	// 1. Copy from cocos2d::GLProgram::parseUniforms
	// 2. ...And Modify
	//
	void GLUniformList::Load( const GLuint program_index )
	{
		mContainer.clear();

		// Query and store uniforms from the program.
		GLint activeUniforms;
		glGetProgramiv( program_index, GL_ACTIVE_UNIFORMS, &activeUniforms );
		if( activeUniforms > 0 )
		{
			GLint length;
			glGetProgramiv( program_index, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length );
			if( length > 0 )
			{
				GLint uniform_size; /**Number of data type in attribute.*/
				GLenum uniform_type; /**Data type of the attribute.*/
				GLchar* uniform_name = (GLchar*)alloca( length + 1 );

				for( int i = 0; i < activeUniforms; ++i )
				{
					// Query uniform info.
					glGetActiveUniform( program_index, i, length, nullptr, &uniform_size, &uniform_type, uniform_name );
					uniform_name[length] = '\0';

					CCLOG( "Checked Name : %s, %d", uniform_name, uniform_type );

					// Only add uniforms that are not built-in.
					// The ones that start with 'CC_' are built-ins
					if( strncmp( "CC_", uniform_name, 3 ) != 0 )
					{
						// remove possible array '[]' from uniform name
						if( length > 3 )
						{
							char* c = strrchr( uniform_name, '[' );
							if( c )
							{
								*c = '\0';
							}
						}

						GLenum __gl_error_code = glGetError();
						if( __gl_error_code != GL_NO_ERROR )
						{
							CCLOG( "error: 0x%x  uniformName: %s", (int)__gl_error_code, uniform_name );
						}
						assert( __gl_error_code == GL_NO_ERROR );

						mContainer[std::string( uniform_name )] = uniform_type;
					}
				}
			}
		}
		else
		{
			GLchar ErrorLog[1024];
			glGetProgramInfoLog( program_index, sizeof( ErrorLog ), nullptr, ErrorLog );
			CCLOG( "Error linking shader program: '%s'\n", ErrorLog );
		}
	}
}
