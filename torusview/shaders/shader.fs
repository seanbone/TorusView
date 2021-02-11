#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;

uniform sampler2D texture1;

vec3 lightDir = vec3(-1.0, -1.0, 0.0);
vec3 ambientLight = vec3(0.3);

void main() {
   vec2 center = vec2(0.5, 0.5);
   float dist = distance(texCoord, center);
   float vis = abs(1 - dist);

   lightDir /= length(lightDir);
   float fac = max(0.0, dot(normal, lightDir));

   vec4 texValue = texture(texture1, texCoord);
   //if (texValue.a < 0.5)
   //   discard;

   vec3 baseColor = texValue.xyz;
   vec3 diffuse = baseColor * fac;
   vec3 ambient = baseColor * ambientLight;
   
   FragColor = vec4(min(vec3(1.0), diffuse + ambient), texValue.a);
}
