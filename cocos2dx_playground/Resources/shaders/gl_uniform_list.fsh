#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec4 u_additiveColor;
uniform vec2 u_addtiveTexCoord;
uniform float u_holdColorR;
uniform float u_holdColorG;

void main()
{
    gl_FragColor = vec4( u_holdColorR, u_holdColorG, mix( v_fragmentColor * texture2D(CC_Texture0, v_texCoord + u_addtiveTexCoord), u_additiveColor, 0.5 ).ba );
}