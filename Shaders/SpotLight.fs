#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 N;
in vec4 tpos;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec4 lpos;
uniform vec4 cpos;

uniform vec3 coeffs;
uniform vec3 ldir;
uniform vec2 coeffsSpot;


void main()
{
    vec3 distv = lpos.xyz - tpos.xyz;
    float dist = length(distv);
    float att = 1/(coeffs.x + (coeffs.y*dist) + (coeffs.z*dist*dist));

    vec3 norm = normalize(N);

    vec3 lightv =normalize(distv);
    float cosH = cos(radians(coeffsSpot.x));
    float cosL = cos(radians(coeffsSpot.y));
    float dotL = dot(lightv, ldir);
    float resVal;

    if(dotL > cosH){
        resVal = 1;
    }
    else if(dotL > cosL){
        resVal = 1 - (cosH - dotL)/(cosH - cosL);
    }
    else{
        resVal = 0;
    }

    vec3 E = normalize(cpos.xyz - tpos.xyz);
    vec3 halfvec =  normalize(lightv + E);

    vec4 amb, spec, diff;
    
 
    amb =vec4(0.1,0.1,0.1,1.0);

    spec = vec4(0.9,0.9,0.9,1.0) * pow(max(dot(norm,halfvec),0.0),30.0);
	
    diff = vec4(1.0,1.0,1.0,1.0) * max(dot(norm,lightv), 0.0);

    
    vec4 t2color;
    vec4 t1color;
    
    t2color = texture(texture2, TexCoord);
    t2color *= t2color.a;
    t1color.a = 1.0;
    t1color = texture(texture1, TexCoord) * (1.0f - t2color.a);
    
    FragColor =  (t1color + t2color) * (resVal*att*diff + resVal*att*spec + amb );
    //FragColor =  (vec4(1.0, 1.0, 1.0, 1.0)) * (att);

    //FragColor = spec;
    
}