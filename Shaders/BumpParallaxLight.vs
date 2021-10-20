#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_out; 
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal;

void main()
{
    
    vs_out.FragPos = vec4(model * vec4(aPos, 1.0)).xyz;

    vec3 T = normalize(vec3(normal * aTangent));
    vec3 B = normalize(vec3(normal * aBitangent));
    vec3 N = normalize(vec3(normal * aNormal)); 
    vs_out.TBN = mat3(T, B, N);
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.TexCoords= vec2(aTexCoord.x, aTexCoord.y);
} 