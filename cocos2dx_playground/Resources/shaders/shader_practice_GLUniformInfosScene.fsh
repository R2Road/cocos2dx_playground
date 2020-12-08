#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec4 u_additiveColor;
uniform vec3 u_addtiveTexCoord;
uniform vec2 u_holdColorR;
uniform float u_holdColorG;
uniform sampler2D u_texture2D;
uniform int u_intTest;

void main()
{
	gl_FragColor = vec4(
		u_holdColorR.r + texture2D( u_texture2D, v_texCoord ).r
		, u_holdColorG + u_intTest
		, mix( v_fragmentColor * texture2D(CC_Texture0, v_texCoord + u_addtiveTexCoord.xy), u_additiveColor, 0.8 ).ba
	);
}