
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	 
	float cutOff;
	float outCutOff;
    float constant;
    float linear;
    float quadratic;
};

varying vec3 Normal;
varying vec3 FragPos;  
varying vec2 texCoord;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float ep = light.cutOff - light.outCutOff;
	float intensity = clamp((theta - light.outCutOff) / ep, 0.0, 1.0);

	vec3 ambient = light.ambient * vec3(texture2D(material.diffuse, texCoord));

	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture2D(material.diffuse, texCoord)) * intensity;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture2D(material.specular, texCoord)) * intensity;

	gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}