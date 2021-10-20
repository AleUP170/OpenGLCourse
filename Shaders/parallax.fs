#version 330 core
out vec4 FragColor;
  
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in; 
  
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec4 lpos;
uniform vec4 cpos;

void main()
{
    float height = texture(texture2, fs_in.TexCoords).r;
    float v = height * 0.03 - 0.015; 
    
    vec3 E = cpos.xyz - fs_in.FragPos;
    mat3 TBNtran = transpose(fs_in.TBN);
    
    E = normalize(TBNtran * E);
    
    vec2 newCoords = fs_in.TexCoords + (E.xy * v); 

    vec4 t1color;

    t1color.a = 1.0;
    t1color = texture(texture1, newCoords) ;
    //t1color = texture(texture2, fs_in.TexCoords) ;
    
    FragColor = t1color;
    
}