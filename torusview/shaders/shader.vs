#version 330 core
#define M_PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
out vec3 normal;

float R = 1;
float r = 0.4;

// Transform the [0;1]x[0;1] xz-plane into a torus
void main()
{
/*
   vec3 pos = aPos;

   float alpha = pos.x;
   float beta = pos.y;

   vec3 p1 = vec3(0.0);
   p1.x =  R*cos(2*M_PI*alpha);
   p1.z = -R*sin(2*M_PI*alpha);

   vec3 dir1 = p1 / length(p1);
   vec3 dir2 = dir1 * cos(2*M_PI*beta);
   dir2.y = sin(2*M_PI*beta);
   dir2 /= length(dir2);

   p1 -= dir2 * r;
*/

   //gl_Position = projection * view * model * vec4(p1, 1.0f);
   gl_Position = projection * view * model * vec4(aPos, 1.0f);

   texCoord = aTexCoord;
   normal = aNormal;
   //normal = dir2;
}
