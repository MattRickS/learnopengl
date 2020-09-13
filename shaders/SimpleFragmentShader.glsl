#version 330 core
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
	// Ambient
	vec3 ambient = material.ambient * light.ambient;

	vec3 norm = normalize(normal);
	
	// Diffuse
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(0.0, dot(norm, lightDir));
	vec3 diffuse = material.diffuse * diff * light.diffuse;

	// Specular
	vec3 viewDir = normalize(fragPos);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light.specular;
	
	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
}