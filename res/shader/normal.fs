#version 330 core

uniform vec2 light_pos;

uniform sampler2D texture0;

in Data
{
	vec4 v_color;
	vec2 v_uv;
	vec4 v_pos;
} fs_in;


void main()
{
	float intensity = 50.0 / length(fs_in.v_pos.xy - light_pos);
	// float alpha = texture2D(texture0, fs_in.v_uv).r;
	// gl_FragColor = vec4(1.0, 1.0, 1.0, alpha) * fs_in.v_color;

	
	vec4 s_color = texture2D(texture0, fs_in.v_uv);
	gl_FragColor = s_color * fs_in.v_color;
	// gl_FragColor *= intensity;
}
