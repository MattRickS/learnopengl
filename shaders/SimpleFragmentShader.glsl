#version 330 core
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 lightPos;
uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	vec3 ambient = ambientStrength * lightColor;
	vec3 norm = normalize(normal);

	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(0.0, dot(norm, lightDir));
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(fragPos);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	fragColor = vec4(result, 1.0);
}