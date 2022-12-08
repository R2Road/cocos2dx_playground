#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float u_customAlpha;

void main()
{
	vec4 default_color =  v_fragmentColor * texture2D(CC_Texture0, v_texCoord );
	default_color.r = ( u_customAlpha > 0.1f ? 1.f : 0.f );

	default_color.g = 0.f;
	default_color.b = 0.f;
	//default_color.a = 0.f;

	//default_color.g = default_color.g * u_customAlpha;
	//default_color.b = default_color.b * u_customAlpha;
	//default_color.a = default_color.a * u_customAlpha;

	gl_FragColor = default_color;
}