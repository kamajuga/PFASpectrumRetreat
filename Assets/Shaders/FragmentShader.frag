#version 330 core

#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

struct Directional
{
	vec3 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct Point
{
	vec3 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float constant;
	float linear;
	float quadratic;
};

struct Spot
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float constant;
	float linear;
	float quadratic;
};

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform Material material;

uniform int nbSpotLights;
uniform int nbPointLights;

uniform Directional dLight;
uniform Point pLight[MAX_POINT_LIGHTS];
uniform Spot sLight[MAX_SPOT_LIGHTS];

uniform vec3 CameraPos;

uniform sampler2D u_Texture;

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos; 

out vec4 FragColor;

vec4 CalcDirectionalLight(Directional light, vec3 normal, vec3 viewDir, vec4 texColor)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular (Blinn-Phong) - FIXED
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec4 ambient = light.ambient * texColor * material.ambient;
    vec4 diffuse = light.diffuse * diff * texColor * material.diffuse;
    vec4 specular = light.specular * spec * material.specular;

    return ambient + diffuse + specular;
}

vec4 CalcPointLight(Point light, vec3 normal, vec3 viewDir, vec4 texColor)
{
	vec3 lightDir = normalize(light.position - FragPos);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular (Blinn-Phong) - FIXED
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    // Attenuation
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec4 ambient = light.ambient * texColor * material.ambient;
    vec4 diffuse = light.diffuse * diff * texColor * material.diffuse;
    vec4 specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular) * attenuation;
};

vec4 CalcSpotLight(Spot light, vec3 normal, vec3 viewDir, vec4 texColor)
{
   vec3 lightDir = normalize(light.position - FragPos);
    
    // Debug: Test if we're in the basic light range first
    float dist = length(light.position - FragPos);
    if (dist > 100.0) return vec4(0.0); // Skip if too far
    
    // Calculate spot light angle and intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = max(light.cutOff - light.outerCutOff, 0.001);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    // Diffuse component
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular component (Blinn-Phong) - FIXED
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    
    // Distance attenuation
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    
    // Calculate lighting components
    vec4 ambient = light.ambient * texColor * material.ambient;
    vec4 diffuse = light.diffuse * diff * texColor * material.diffuse;
    vec4 specular = light.specular * spec * material.specular;
    
    // Apply attenuation and intensity correctly
    vec4 result = ambient * intensity;
    result += (diffuse + specular) * attenuation * intensity;
    
    return result;
}
void main()
{
	vec4 color = vec4(0.0);

    vec3 viewDir = normalize(CameraPos - FragPos); 

    //vec4 texColor = texture(u_Texture, texCoord) * material.diffuse;
	vec4 texColor = vec4(1.0, 1.0, 1.0, 1.0); // Blanc pur
    // Directional
    color += CalcDirectionalLight(dLight, Normal, viewDir, texColor);

    // Point Lights
    for (int i = 0; i < nbPointLights; ++i)
        color += CalcPointLight(pLight[i], Normal, viewDir, texColor);

    // Spot Lights
    for (int i = 0; i < nbSpotLights; ++i)
        color += CalcSpotLight(sLight[i], Normal, viewDir, texColor);

    //FragColor = vec4(color.rgb, 0.1);
	//FragColor = color;
    FragColor = vec4(color.rgb, material.ambient.a);
}