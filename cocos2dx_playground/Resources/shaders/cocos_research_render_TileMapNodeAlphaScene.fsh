#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
	vec4 default_color =  v_fragmentColor * texture2D(CC_Texture0, v_texCoord );
	default_color.r = default_color.r * default_color.a;
	default_color.g = default_color.g * default_color.a;
	default_color.b = default_color.b * default_color.a;

	gl_FragColor = default_color;
}