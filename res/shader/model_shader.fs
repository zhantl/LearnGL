#version 330 core
//ƽ�й�
struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//���Դ
struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//�۹��
struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
	vec3 Normal;
	mat3 TBN;
} fs_in;

#define NR_POINT_LIGHTS 4

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

	vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);   
	normal = normalize(fs_in.TBN * normal);
	
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	// ƽ�й�
	vec3 result = CalcDirLight(dirLight, normal, viewDir);
	// ������Դ
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], normal, fs_in.FragPos, viewDir);
	// ����۹��
	result += CalcSpotLight(spotLight, normal, fs_in.FragPos, viewDir);

	gl_FragColor = vec4(result, 1.0);
}

// ����ƽ�й�
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// ɢ��
	float diff = max(dot(normal, lightDir), 0.0);
	// ����
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	// ����
	vec3 ambient = light.ambient * vec3(texture2D(texture_diffuse1, fs_in.TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture2D(texture_diffuse1, fs_in.TexCoords));
	vec3 specular = light.specular * spec * vec3(texture2D(texture_specular1, fs_in.TexCoords));
	return (ambient + diffuse + specular);
}

// ������Դ
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); //�����˥��
	vec3 ambient = light.ambient * vec3(texture2D(texture_diffuse1, fs_in.TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture2D(texture_diffuse1, fs_in.TexCoords));
	vec3 specular = light.specular * spec * vec3(texture2D(texture_specular1, fs_in.TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

// ����۹��
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	vec3 ambient = light.ambient * vec3(texture2D(texture_diffuse1, fs_in.TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture2D(texture_diffuse1, fs_in.TexCoords));
	vec3 specular = light.specular * spec * vec3(texture2D(texture_specular1, fs_in.TexCoords));
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}