#version 430 core

uniform sampler2D texture;

in vec4 fragCol;
in vec2 fragTexCoord;
out vec4 outColor;

void main()
{
   float a = texture2D(texture, fragTexCoord).r;
   float c =  max(0.0,min(1.0,a));
   outColor= vec4(fragCol.rgb*c,c);
}
