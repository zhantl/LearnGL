varying vec3 Position;
varying vec3 Normal;

uniform samplerCube skybox;
uniform vec3 cameraPos;

void main()
{
	vec3 I = normalize(Position - cameraPos);

	vec3 R = reflect(I, normalize(Normal));//∑¥…‰

	float ratio = 1.00 / 1.52;
	//vec3 R = refract(I, normalize(Normal), ratio);//’€…‰
	
	gl_FragColor = vec4(textureCube(skybox, R).rgb, 1.0);
}
