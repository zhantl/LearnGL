#version 330 core

uniform sampler2D texture0;

in vec3 w_pos;

//等距柱状投影到立方体贴图
const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
	vec2 uv = SampleSphericalMap(normalize(w_pos));
	vec3 s_color = texture2D(texture0, uv).rgb;
	gl_FragColor = vec4(s_color, 1.0);
}
