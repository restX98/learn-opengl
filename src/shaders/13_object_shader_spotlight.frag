#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

// With Soft edges
void main() {
  // ambient lighting
  vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

  // diffuse 
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

  // specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

  // spotlight (soft edges)
  float theta = dot(lightDir, normalize(-light.direction)); 
  float epsilon = (light.cutOff - light.outerCutOff);
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
  diffuse  *= intensity;
  specular *= intensity;
    
  // attenuation
  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  vec3 result = (ambient + diffuse + specular) * attenuation;

  FragColor = vec4(result, 1.0);
}


// Without Soft edges
// void main() {
//   vec3 lightDir = normalize(light.position - FragPos);

//   float theta = dot(lightDir, normalize(-light.direction)); 

//   if(theta > light.cutOff) {
//     // ambient lighting
//     vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

//     // diffuse 
//     vec3 norm = normalize(Normal);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

//     // specular
//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

//     // attenuation
//     float distance = length(light.position - FragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

//     vec3 result = (ambient + diffuse + specular) * attenuation;

//     FragColor = vec4(result, 1.0);
//   } else {
//     // else, use ambient light so scene isn't completely dark outside the spotlight.
//     FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
//   }
// }
