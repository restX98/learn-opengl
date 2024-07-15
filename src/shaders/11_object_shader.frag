#version 330 core

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
  // ambient lighting
  vec3 ambient = light.ambient * material.ambient;

  // diffuse 
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos); // Vector from fragment to light
  float diff = max(dot(norm, lightDir), 0.0); // quantity of reflection of the light on the fragment
  vec3 diffuse = light.diffuse * (diff * material.diffuse);

  // specular
  vec3 viewDir = normalize(viewPos - FragPos); // Vector from fragment to viewer 
  vec3 reflectDir = reflect(-lightDir, norm); // Vector that rappresents the reflection of the light (-lightDir because of the function reflects that work as is)
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // Quantity of reflextion of the light in the viewer direction
  // The pow is applied to a number that is greater than 0 and lower or equal than 1, then the result will be lower than the param value
  // Then, the shininest figure will have a specific point of reflection that corrispond to the angle near to 0.
  // The less shinest fidure will have a vaster reflection area.
  vec3 specular = light.specular * (spec * material.specular);

  vec3 result = ambient + diffuse + specular;

  FragColor = vec4(result, 1.0);
}
