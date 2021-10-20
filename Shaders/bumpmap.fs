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
    vec3 norm = texture(texture2, fs_in.TexCoords).rgb;
    norm = norm * 2.0 - 1.0;
    norm = normalize(fs_in.TBN * norm);
    
    vec3 lightv =normalize(lpos.xyz - fs_in.FragPos);
    vec3 E = normalize(cpos.xyz - fs_in.FragPos);
    vec3 halfvec =  normalize(lightv + E);

    vec4 amb, spec, diff;
    
    amb =vec4(0.04,0.04,0.04,1.0);
    spec = vec4(0.9,0.9,0.9,1.0) * pow(max(dot(norm,halfvec),0.0),10.0);
    diff = vec4(1.0,1.0,1.0,1.0) * max(dot(norm,lightv), 0.0);

    //vec4 t2color;
    vec4 t1color;
    
    t1color.a = 1.0;
    t1color = texture(texture1, fs_in.TexCoords) ;
    
    FragColor =  (t1color) * (diff + spec);
    
}