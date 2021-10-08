#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    
    
    vec4 t2color;
    vec4 t1color;
    
    t2color = texture(texture2, TexCoord);
    t2color *= t2color.a;
    t1color.a = 1.0;
    t1color = texture(texture1, TexCoord) * (1.0f - t2color.a);
    
    
    FragColor = t1color + t2color;
    
}