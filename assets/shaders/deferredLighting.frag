#version 330 core
layout (location = 0) out vec4 FragColor;

#define MAX_LIGHTS_DIR 1
#define MAX_LIGHTS_POINT 6
#define MAX_LIGHTS_SPOT 2

struct LightDirectional
{
    vec3 direction;
    vec3 color;
    
    float intensity;

    int castShadows;
};

struct LightPoint
{
    vec3 position;
    vec3 color;
    
    float intensity;
	float dropoff;
    float range;

    int castShadows;
};

struct LightSpot
{
    vec3 position;
    vec3 direction;
    vec3 color;
    
    float intensity;
	float dropoff;
    float range;
	
	float innerAngle;
	float outerAngle;

    int castShadows;
};


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMaps;
uniform sampler2D depthMap;
uniform samplerCube depthCubemap[MAX_LIGHTS_POINT + MAX_LIGHTS_SPOT];

in vec2 TexCoords;

uniform int numLightsDirectional;
uniform int numLightsPoint;
uniform int numLightsSpot;
uniform LightDirectional lightsDirectional[MAX_LIGHTS_DIR];
uniform LightPoint lightsPoint[MAX_LIGHTS_POINT];
uniform LightSpot lightsSpot[MAX_LIGHTS_SPOT];

uniform vec3 cameraPosition;

//GBuffer variables
vec3 FragPos;
vec3 Normal;

//Material variables
vec3 Albedo;
float Rough;
float Metal;
float AO;


//Depth
float Depth;
uniform mat4 lightSpaceMatrix;
vec4 FragPosLightSpace;
//Variables
vec3 viewDir;
// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);


const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


float ShadowMap(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
	if (projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
		return 1.0f;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-lightsDirectional[0].direction);
    float bias = 0.001;		//max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); - This was terrible.

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
	
	//Invert
    return 1.0f - shadow;
}


float ShadowCubeMap(vec3 fragPos, vec3 lightPos, float range, samplerCube map)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
	
	float shadow = 0.0;
    float bias = 0.15;
    float far_plane = range;
    int samples = 20;
    float viewDistance = length(cameraPosition - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(map, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    
    return 1.0 - shadow;
}


vec3 DirectionalLighting(LightDirectional light)
{
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, Albedo, Metal);

    // calculate per-light radiance
	vec3 lightDir = normalize(-light.direction);
    vec3 H = normalize(viewDir + lightDir);

    vec3 radiance = clamp(light.color, 0.0, 1.0) * light.intensity;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(Normal, H, Rough);   
    float G   = GeometrySmith(Normal, viewDir, lightDir, Rough);      
    vec3 F    = fresnelSchlick(clamp(dot(H, viewDir), 0.0, 1.0), F0);
           
    vec3 nominator    = NDF * G * F; 
    float denominator = 4 * max(dot(Normal, viewDir), 0.0) * max(dot(Normal, lightDir), 0.0);
    vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - Metal;	  

    // scale light by NdotL
    float NdotL = max(dot(Normal, lightDir), 0.0);        
	

	//Result
    vec3 result = (kD * Albedo / PI + specular) * radiance * NdotL;

	//Shadow
	float shadow = 1.0f;
	if (light.castShadows > 0)
		shadow = ShadowMap(FragPosLightSpace); 
	return result * shadow;
}


vec3 PointLighting(int i)
{
	LightPoint light = lightsPoint[i];
  
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, Albedo, Metal);

    // calculate per-light radiance
	vec3 lightDir = normalize(light.position - FragPos);
    vec3 H = normalize(viewDir + lightDir);

	//Attenuation
    float dist = length(light.position - FragPos);
	if (dist > light.range)
		return vec3(0.0);

	float linear = (1.0 / light.dropoff) * dist * 0.09;
	float quadratic = (1.0 / light.dropoff) * dist * dist * 0.032;
	float attenuation = 1.0 / ((1.0 / light.intensity) + linear + quadratic);
        
    vec3 radiance = clamp(light.color, 0.0, 1.0) * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(Normal, H, Rough);   
    float G   = GeometrySmith(Normal, viewDir, lightDir, Rough);      
    vec3 F    = fresnelSchlick(clamp(dot(H, viewDir), 0.0, 1.0), F0);
           
    vec3 nominator    = NDF * G * F; 
    float denominator = 4 * max(dot(Normal, viewDir), 0.0) * max(dot(Normal, lightDir), 0.0);
    vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - Metal;	  

    // scale light by NdotL
    float NdotL = max(dot(Normal, lightDir), 0.0);        
	

	//Result
    vec3 result = (kD * Albedo / PI + specular) * radiance * NdotL;

	//Shadow
	float shadow = 1.0f;
	if (light.castShadows > 0)
		shadow = ShadowCubeMap(FragPos, light.position, light.range, depthCubemap[i]);
	return result * shadow;
}


vec3 SpotLighting(int i)
{
	LightSpot light = lightsSpot[i];
  
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, Albedo, Metal);

    // calculate per-light radiance
	vec3 lightDir = normalize(light.position - FragPos);
    vec3 H = normalize(viewDir + lightDir);

	//Attenuation
    float dist = length(light.position - FragPos);
	if (dist > light.range)
		return vec3(0.0);

	float linear = (1.0 / light.dropoff) * dist * 0.09;
	float quadratic = (1.0 / light.dropoff) * dist * dist * 0.032;
	float attenuation = 1.0 / ((1.0 / light.intensity) + linear + quadratic);
        
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.innerAngle - light.outerAngle);
    float intensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0);

    vec3 radiance = clamp(light.color, 0.0, 1.0) * attenuation * intensity;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(Normal, H, Rough);   
    float G   = GeometrySmith(Normal, viewDir, lightDir, Rough);      
    vec3 F    = fresnelSchlick(clamp(dot(H, viewDir), 0.0, 1.0), F0);
           
    vec3 nominator    = NDF * G * F; 
    float denominator = 4 * max(dot(Normal, viewDir), 0.0) * max(dot(Normal, lightDir), 0.0);
    vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - Metal;	  

    // scale light by NdotL
    float NdotL = max(dot(Normal, lightDir), 0.0);        
	

	//Result
    vec3 result = (kD * Albedo / PI + specular) * radiance * NdotL;

	//Shadow
	float shadow = 1.0f;
	if (light.castShadows > 0)
		shadow = ShadowCubeMap(FragPos, light.position, light.range, depthCubemap[MAX_LIGHTS_POINT + i]);
	return result * shadow;

}



vec3 Lighting()
{
	//Initalize lighting value.
	vec3 lighting = vec3(0.0f);
	
	for(int i = 0; i < numLightsDirectional && i < MAX_LIGHTS_DIR; i++)
		lighting += DirectionalLighting(lightsDirectional[i]);
		
    for(int i = 0; i < numLightsPoint && i < MAX_LIGHTS_POINT; i++)
		lighting += PointLighting(i);

    for(int i = 0; i < numLightsSpot && i < MAX_LIGHTS_SPOT; i++)
		lighting += SpotLighting(i);

	return lighting;
}




void main()
{
    //Get GBuffer data
    FragPos = texture(gPosition, TexCoords).rgb;
    Normal = texture(gNormal, TexCoords).rgb;
    Albedo = pow(texture(gAlbedo, TexCoords).rgb, vec3(2.2));
    Rough = texture(gMaps, TexCoords).r;
    Metal = texture(gMaps, TexCoords).g;
	AO = texture(gMaps, TexCoords).b;

	//Depth
	Depth = texture(depthMap, TexCoords).r;
    viewDir  = normalize(cameraPosition - FragPos);
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	
	
	//Lighting
    vec3 ambient = vec3(0.01) * Albedo * AO;
    vec3 result = ambient + Lighting();


    FragColor = vec4(result, 1.0);
}
