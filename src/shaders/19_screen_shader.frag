#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

vec4 inversion();
vec4 grayscale_avg();
vec4 grayscale_accurate();

vec4 sharper_kernel();
vec4 blur_kernel();
vec4 edge_detection_kernel();

void main() {
  // vec3 col = texture(screenTexture, TexCoords).rgb;
  // FragColor = vec4(col, 1.0);

  // FragColor = inversion();

  // FragColor = grayscale_avg();

  // FragColor = grayscale_accurate();

  // Kernel effects:
  // Another advantage about doing post-processing on a single texture image is
  // that we can sample color values from other parts of the texture not specific
  // to that fragment.
  // A  kernel (or convolution matrix) is a small matrix-like array of values
  // centered on the current pixel that multiplies surrounding pixel values by
  // its kernel values and adds them all together to form a single value.
  
  // FragColor = sharper_kernel();
  // FragColor = blur_kernel();
  FragColor = edge_detection_kernel();
}

vec4 inversion() {
  return vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);;
}

vec4 grayscale_avg() {
  vec4 fragColor = texture(screenTexture, TexCoords);
  float average = (fragColor.r + fragColor.g + fragColor.b) / 3.0;
  return vec4(average, average, average, 1.0);
}

// The human eye tends to be more sensitive to green colors and the least to blue.
//  So to get the most physically accurate results we'll need to use weighted channels:
vec4 grayscale_accurate() {
  vec4 fragColor = texture(screenTexture, TexCoords);
  float average = 0.2126 * fragColor.r + 0.7152 * fragColor.g + 0.0722 * fragColor.b;
  return vec4(average, average, average, 1.0);
}

vec4 sharper_kernel() {
  vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right    
  );

  float kernel[9] = float[](
      -1, -1, -1,
      -1,  9, -1,
      -1, -1, -1
  );
  
  vec3 sampleTex[9];
  for(int i = 0; i < 9; i++)
  {
      sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
  }
  vec3 col = vec3(0.0);
  for(int i = 0; i < 9; i++)
      col += sampleTex[i] * kernel[i];
  
  return vec4(col, 1.0);
}

vec4 blur_kernel() {
  vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right    
  );

  float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);
  
  vec3 sampleTex[9];
  for(int i = 0; i < 9; i++)
  {
      sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
  }
  vec3 col = vec3(0.0);
  for(int i = 0; i < 9; i++)
      col += sampleTex[i] * kernel[i];
  
  return vec4(col, 1.0);
}

vec4 edge_detection_kernel() {
  vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right    
  );

  float kernel[9] = float[](
      1, 1, 1,
      1, -8, 1,
      1, 1, 1
  );
  
  vec3 sampleTex[9];
  for(int i = 0; i < 9; i++)
  {
      sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
  }
  vec3 col = vec3(0.0);
  for(int i = 0; i < 9; i++)
      col += sampleTex[i] * kernel[i];
  
  return vec4(col, 1.0);
}
