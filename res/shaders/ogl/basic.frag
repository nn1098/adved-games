#version 430 core

// Interpolated values from the vertex shaders
in vec4 fragmentColor;

// Ouput data
out vec4 color;

void main() {

  // Output color = color specified in the vertex shader,
  // interpolated between all 3 surrounding vertices
  color = fragmentColor;
  // color = vec4(1.0f,0.0f,0.0f,0.7f);
}
