#version 330 core

in vec2 TexCoords;

float near = 0.1;
float far = 100.0;

uniform sampler2D texture1;

float linerDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	//float d = linerDepth(gl_FragCoord.z);
	gl_FragColor = texture2D(texture1, TexCoords);// vec4(vec3(d), 1.0);// 
}
