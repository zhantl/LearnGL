#version 330 core

uniform sampler2D texture0;

in Data
{
	vec4 v_color;
	vec2 v_uv;
	vec4 v_pos;
} fs_in;


void main()
{
	float alpha = texture2D(texture0, fs_in.v_uv).r;
	gl_FragColor = vec4(1.0, 1.0, 1.0, alpha) * fs_in.v_color;
}
