#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

float near = 0.1; 
float far  = 2.0; 

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC 
    float linearDepth = (2.0 * near * far) / (far + near - z * (far - near));
    return (linearDepth - near) / (far - near); //  linear
    // return 1- (1/(1+depth));	
}

void main() {
  float depth = LinearizeDepth(gl_FragCoord.z); // divide by far for demonstration
  FragColor = vec4(vec3(depth), 1.0);
  // FragColor = texture(texture1, TexCoords) + vec4(vec3(depth), 1.0);
}
