#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform sampler2D bumpTexture;
uniform sampler2D normalTexture;

uniform vec3 lightPosition;

uniform vec3 coeffs;
uniform vec3 ldir;
uniform vec2 coeffsSpot;

uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, float mBias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // check whether current frag pos is in shadow
    //float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    
    //shadow acne
    //float shadow = currentDepth - mBias > closestDepth  ? 0.8 : 0.0; 
    //Oversampling
    //if(projCoords.z > 1.0)
    //    shadow = 0.0;
        
    //PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - mBias > pcfDepth ? 0.7 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}  

void main()
{       
    // Parallax map
    
    //float height = texture(bumpTexture, fs_in.TexCoords).r;
    //float v = height * 0.03 - 0.015; 
    //
    //vec3 E = viewPos.xyz - fs_in.FragPos;
    //mat3 TBNtran = transpose(fs_in.TBN);
    //
    //E = normalize(TBNtran * E);
    //
    //vec2 newCoords = fs_in.TexCoords + (E.xy * v); 

    // Normal map 

    //vec3 norm = texture(normalTexture, newCoords).rgb;
    //norm = norm * 2.0 - 1.0;
    //norm = normalize(fs_in.TBN * norm);
    
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPosition - fs_in.FragPos);

    // Spotlight
    vec3 distV =(lightPosition - fs_in.FragPos);
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

    // Blinn - Phong
    vec3 E = normalize(viewPos.xyz - fs_in.FragPos);
    vec3 halfvec =  normalize(lightv + E);

    vec4 amb, spec, diff;
    
    amb =vec4(0.1,0.1,0.1,1.0);
    spec = vec4(0.9,0.9,0.9,1.0) * pow(max(dot(normal,halfvec),0.0),30.0);
    diff = vec4(1.0,1.0,1.0,1.0) * max(dot(normal,lightv), 0.0);

 
    // calculate shadow
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);       
    
    
    vec3 lighting = (1.0 - shadow) * color * (resVal*att*diff.xyz + resVal*att*spec.xyz + amb.xyz);    
    
    FragColor = vec4(lighting, 1.0);
}