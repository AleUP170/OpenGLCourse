#version 330 core
out vec4 FragColor;
  

in vec2 TexCoord;
in vec3 N;
in vec4 tpos;


uniform sampler2D texture1;
uniform sampler2D texture2;

//uniform vec4 lpos;
uniform vec3 lvec;
uniform vec4 cpos;

uniform int type;

void main()
{

    vec3 norm = normalize(N);
    //vec3 lightv =normalize(lpos.xyz - tpos.xyz);

    vec3 lightv = normalize(lvec);
    vec3 E = normalize(cpos.xyz - tpos.xyz);
    vec3 halfvec =  normalize(lightv + E);

    vec4 amb, spec, diff;
    
 
    amb =vec4(0.5,0.5,0.5,1.0);

    spec = vec4(0.9,0.9,0.9,1.0) * pow(max(dot(norm,halfvec),0.0),30.0);
	
    diff = vec4(1.0,1.0,1.0,1.0) * max(dot(norm,lightv), 0.0);

    
    vec4 t2color;
    vec4 t1color;
    
    t2color = texture(texture2, TexCoord);
    t2color *= t2color.a;
    t1color.a = 1.0;
    t1color = texture(texture1, TexCoord) * (1.0f - t2color.a);
    
    FragColor =  (t1color + t2color) * (diff + spec + amb );

    //FragColor = spec;
    
}