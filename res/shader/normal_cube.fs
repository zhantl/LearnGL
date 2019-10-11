#version 330 core

uniform sampler2D texture0;

in Data
{
	vec4 v_pos;
	vec2 v_uv;
	vec3 v_normal;
} fs_in;


void main()
{
	vec4 s_color = texture2D(texture0, fs_in.v_uv);
	gl_FragColor = s_color;
}
