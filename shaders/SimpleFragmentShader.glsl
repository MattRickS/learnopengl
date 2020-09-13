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
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
	vec3 norm = normalize(normal);	
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 viewDir = normalize(fragPos);
	vec3 reflectDir = reflect(lightDir, norm);

	float diff = max(0.0, dot(norm, lightDir));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, UV));
	vec3 diffuse = light.diffuse   * diff * vec3(texture(material.diffuse, UV));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, UV));
	vec3 emission = vec3(texture(material.emission, UV) * texture(material.specular, UV));
	
	vec3 result = ambient + diffuse + specular + emission;
	fragColor = vec4(result, 1.0);
}