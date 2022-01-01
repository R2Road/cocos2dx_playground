varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D u_texture;                                         
		
void main()			
{
	vec4 vOrigColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
	float gray = dot(vOrigColor.rgb, vec3(0.1, 0.1, 0.1));
	gl_FragColor = vec4(gray, gray, gray, vOrigColor.a);
}	