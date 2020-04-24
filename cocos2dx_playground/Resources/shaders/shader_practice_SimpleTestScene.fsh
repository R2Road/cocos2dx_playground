#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float u_testColor;

void main()
{
	vec4 default_color =  v_fragmentColor * texture2D(CC_Texture0, v_texCoord );
	default_color.g = default_color.a > 0.0 ? default_color.g + u_testColor : default_color.g;

	gl_FragColor = default_color;
}