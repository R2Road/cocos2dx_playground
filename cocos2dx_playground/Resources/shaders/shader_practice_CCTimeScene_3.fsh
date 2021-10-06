#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
	vec4 default_color =  v_fragmentColor * texture2D(CC_Texture0, v_texCoord );
	default_color.r = default_color.a > 0.f ? ( sin( CC_Time[3] ) + 1.f ) * 0.5f : 0.f;

	gl_FragColor = default_color;
}