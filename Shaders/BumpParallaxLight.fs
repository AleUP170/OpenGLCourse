#version 330 core
out vec4 FragColor;
  
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in; 
  
uniform sampler2D texture1; // Regular texture 
uniform sampler2D texture2; // Height map 
uniform sampler2D texture3; // Normal map 

uniform vec4 lpos;
uniform vec4 cpos;

uniform vec3 coeffs;
uniform vec3 ldir;
uniform vec2 coeffsSpot;

void main()
{
    // Parallax map
    
    float height = texture(texture2, fs_in.TexCoords).r;
    float v = height * 0.03 - 0.015; 
    
    vec3 E = cpos.xyz - fs_in.FragPos;
    mat3 TBNtran = transpose(fs_in.TBN);
    
    E = normalize(TBNtran * E);
    
    vec2 newCoords = fs_in.TexCoords + (E.xy * v); 

    // Normal map 

    vec3 norm = texture(texture3, newCoords).rgb;
    norm = norm * 2.0 - 1.0;
    norm = normalize(fs_in.TBN * norm);
    
    // Spotlight
    vec3 distV =(lpos.xyz - fs_in.FragPos);
    float dist = length(distV);
    float att = 1/(coeffs.x + (coeffs.y*dist) + (coeffs.z*dist*dist));

    vec3 lightv = normalize(distV);
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

    // Componentes

    E = normalize(cpos.xyz - fs_in.FragPos);
    vec3 halfvec =  normalize(lightv + E);
    
    vec4 amb, spec, diff;
    
    amb =vec4(0.04,0.04,0.04,1.0);
    spec = vec4(0.9,0.9,0.9,1.0) * pow(max(dot(norm,halfvec),0.0),10.0);
    diff = vec4(1.0,1.0,1.0,1.0) * max(dot(norm,lightv), 0.0);

    //vec4 t2color;
    vec4 t1color;
    
    t1color.a = 1.0;
    t1color = texture(texture1, newCoords) ;
    FragColor =  (t1color) * (resVal*att*diff + resVal*att*spec + amb);
}