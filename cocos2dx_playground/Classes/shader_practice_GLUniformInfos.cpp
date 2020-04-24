#include "shader_practice_GLUniformInfos.h"

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
	void GLUniformInfos::Load( const GLuint program_index )
	{
		mContainer.clear();

		// Query and store uniforms from the program.
		GLint active_uniform_count;
		glGetProgramiv( program_index, GL_ACTIVE_UNIFORMS, &active_uniform_count );
		if( active_uniform_count <= 0 )
		{
			GLchar ErrorLog[1024];
			glGetProgramInfoLog( program_index, sizeof( ErrorLog ), nullptr, ErrorLog );
			CCLOG( "Error linking shader program: '%s'\n", ErrorLog );
			return;
		}

		GLint active_uniform_max_length;
		glGetProgramiv( program_index, GL_ACTIVE_UNIFORM_MAX_LENGTH, &active_uniform_max_length );
		if( active_uniform_max_length <= 0 )
		{
			return;
		}

		GLint uniform_size; /**Number of data type in attribute.*/
		GLint uniform_location;
		GLenum uniform_type; /**Data type of the attribute.*/
		GLchar* uniform_name = (GLchar*)alloca( active_uniform_max_length + 1 );
		for( int i = 0; i < active_uniform_count; ++i )
		{
			// Query uniform info.
			glGetActiveUniform( program_index, i, active_uniform_max_length, nullptr, &uniform_size, &uniform_type, uniform_name );
			uniform_name[active_uniform_max_length] = '\0';

			CCLOG( "Checked Name : %s, %d", uniform_name, uniform_type );

			// Only add uniforms that are not built-in.
			// The ones that start with 'CC_' are built-ins
			if( strncmp( "CC_", uniform_name, 3 ) == 0 )
			{
				continue;
			}

			// remove possible array '[]' from uniform name : ex) u_Color[0]
			if( active_uniform_max_length > 3 )
			{
				char* c = strrchr( uniform_name, '[' );
				if( c )
				{
					*c = '\0';
				}
			}

			uniform_location = glGetUniformLocation( program_index, uniform_name );

			GLenum __gl_error_code = glGetError();
			if( __gl_error_code != GL_NO_ERROR )
			{
				CCLOG( "error: 0x%x  uniformName: %s", (int)__gl_error_code, uniform_name );
			}
			assert( __gl_error_code == GL_NO_ERROR );

			mContainer.emplace_back( uniform_name, uniform_type, uniform_location );
		}
	}

	const char* GLenum2String( const GLenum gl_enum )
	{
		if( GL_FLOAT == gl_enum )
		{
			return "Float";
		}

		if( GL_FLOAT_VEC2 == gl_enum )
		{
			return "Vec2";
		}
		if( GL_FLOAT_VEC3 == gl_enum )
		{
			return "Vec3";
		}
		if( GL_FLOAT_VEC4 == gl_enum )
		{
			return "Vec4";
		}

		if( GL_SAMPLER_2D == gl_enum )
		{
			return "Sampler2D";
		}

		if( GL_INT == gl_enum )
		{
			return "Int";
		}

		return "o_o";
	}
}
