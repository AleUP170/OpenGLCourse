#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 v;
out vec3 N;
out vec3 L;
  
//uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal;
uniform vec3 lpos;

void main()
{
    
    
    vec4 tpos = model * vec4(aPos, 1.0);

    v = vec3(view * tpos);
    N = normalize(normal * aNormal);
    
    L = normalize( lpos - tpos.xyz);

    //gl_Position = transform * vec4(aPos, 1.0f);
     gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
} 