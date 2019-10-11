#version 330 core

// 材质
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

//辐照度环境贴图
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;

// 点光
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 viewPos;

const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.0;

in Data
{
	vec3 v_pos;
	vec2 v_uv;
	vec3 v_normal;
} fs_in;


// Trowbridge-Reitz GGX
// NDFGGXTR(n, h, r)= r * r / pow(π((n ⋅ h) * (n * h) * (r * r − 1) + 1), 2)
// 估算在受到表面粗糙度的影响下，取向方向与中间向量H一致的微平面的数量 
// 粗燥都的平方会看起来更自然
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); 
}

// k_direct = (r + 1) * (r + 1) / 8
// k_IBL = r * r / 2
// GSchlickGGX(n, v, k)= n ⋅ v / ((n ⋅ v) * (1 − k) + k)
// 描述了微平面自成阴影的属性。
// 当一个平面相对比较粗糙的时候，
// 平面表面上的微平面有可能挡住其他的微平面从而减少表面所反射的光线
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

// Smith’s Schlick-GGX
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Fresnel-Schlick Approximation
// 菲涅尔方程描述的是在不同的表面角下表面所反射的光线所占的比率
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
//带粗糙度参数
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 getNormalFromNormalMap()
{
	vec3 tangentNormal = texture(normalMap, fs_in.v_uv).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.v_pos);
    vec3 Q2  = dFdy(fs_in.v_pos);
    vec2 st1 = dFdx(fs_in.v_uv);
    vec2 st2 = dFdy(fs_in.v_uv);

    vec3 N   = normalize(fs_in.v_normal);
    vec3 T  = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
	vec3 albedo = pow(texture(albedoMap, fs_in.v_uv).rgb, vec3(2.2));
	float metallic = texture(metallicMap, fs_in.v_uv).r;
	float roughness = texture(roughnessMap, fs_in.v_uv).r;
	float ao = texture(aoMap, fs_in.v_uv).r;

    vec3 N = getNormalFromNormalMap();
    vec3 V = normalize(viewPos - fs_in.v_pos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - fs_in.v_pos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - fs_in.v_pos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001); 
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  
        float NdotL = max(dot(N, L), 0.0);        
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }   
    
    // 环境光
    vec3 F = fresnelSchlickRoughness(clamp(dot(N, V), 0.0, 1.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1. - kS;
    kD *= 1.0 - metallic;	
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo; //漫反射

    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F0 * brdf.x + brdf.y); //镜面反射 

    vec3 ambient = (kD * diffuse + specular) * ao;
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0 / 2.2)); 

    gl_FragColor = vec4(color, 1.0);
}
