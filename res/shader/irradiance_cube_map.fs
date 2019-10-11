#version 330 core

uniform samplerCube texture0;

in vec3 w_pos;

const float PI = 3.14159265359;

void main()
{
	vec3 noraml = normalize(w_pos);
	vec3 irradiance = vec3(0.0);

	vec3 up = vec3(0., 1., 0.);
	vec3 right = cross(up, noraml);
	up = cross(noraml, right);

	float sampleDelta = 0.025;
	float nrSamples = 0.;

	for(float phi = 0.; phi < PI * 2.; phi += sampleDelta)
	{
		for(float theta = 0.; theta < PI * 0.5; theta += sampleDelta)
		{
			//半径为1的球体空间坐标  切线空间 z = n 朝上
			vec3 sampleTangent = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			//球体空间坐标转换到世界坐标采样环境贴图
			vec3 sampleWorld = sampleTangent.x * right + sampleTangent.y * up + sampleTangent.z * noraml;
			//cos(theta)入射光线和法线dot，因为这种方式的球体坐标夹角越小的时候采样越密集，所以要乘以sin(theta)以减少权重
			irradiance += texture(texture0, sampleWorld).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}

	irradiance = PI * irradiance / nrSamples;
	gl_FragColor = vec4(irradiance, 1.0);
}
