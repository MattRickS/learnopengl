#version 330 core
in vec3 normal;
in vec3 fragPos;
in vec2 UV;

out vec4 fragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light
{
	int type;

	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

uniform Material material;
uniform Light light;

void main()
{
	vec3 lightDir;
	float attenuation;
	// Directional Light
	if (light.type == 0)
	{
		lightDir = normalize(-light.direction);
		attenuation = 1.0;
	}
	// Positional Light
	else if (light.type == 1)
	{
		lightDir = normalize(light.position - fragPos);
		float distance = length(light.position - fragPos);
		attenuation = 1.0 / (light.constant + light.linear * distance
					  + light.quadratic * distance * distance);
	}
	// Spotlight
	else
	{
		lightDir = normalize(light.position - fragPos);
		float theta = dot(lightDir, normalize(-light.direction));
		if (theta > light.cutOff)
		{
			attenuation = 1.0f;
		}
		else
		{
			float epsilon = light.cutOff - light.outerCutOff;
			attenuation = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		}
	}

	vec3 norm = normalize(normal);	
	vec3 viewDir = normalize(fragPos);
	vec3 reflectDir = reflect(lightDir, norm);

	float diff = max(0.0, dot(norm, lightDir));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, UV));
	vec3 diffuse = light.diffuse   * diff * vec3(texture(material.diffuse, UV)) * attenuation;
	vec3 specular = light.specular * spec * vec3(texture(material.specular, UV)) * attenuation;
	vec3 emission = vec3(texture(material.emission, UV) * texture(material.specular, UV));
	
	vec3 result = ambient + diffuse + specular; // + emission;
	fragColor = vec4(result, 1.0);
}