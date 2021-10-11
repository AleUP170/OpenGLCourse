#version 330 core
out vec4 FragColor;
  
//in vec3 ourColor;
in vec2 TexCoord;
in vec3 v;
in vec3 N;
in vec3 L;

uniform sampler2D texture1;
uniform sampler2D texture2;

//uniform vec3 lpos;

void main()
{

    vec3 tmpL = normalize(L); 
    vec3 E = normalize(-v);
    vec3 R = normalize(-reflect(tmpL,N));

    vec4 amb, spec, diff;
    
 
    amb =vec4(0.08,0.08,0.08,1.0);

    //  Ks * (R dot V)^n

    spec = vec4(0.9,0.9,0.9,1.0) * pow(max(dot(R,E),0.0),30.0);

    // Kd * (N dot L) 
	
    diff = vec4(1.0,1.0,1.0,1.0) * max(dot(N,tmpL), 0.0);

    
    
    vec4 t2color;
    vec4 t1color;
    
    t2color = texture(texture2, TexCoord);
    t2color *= t2color.a;
    t1color.a = 1.0;
    t1color = texture(texture1, TexCoord) * (1.0f - t2color.a);
    
    
    //FragColor = t1color + t2color;
    //FragColor = vec4(L, 1.0);
    FragColor =  (t1color + t2color) * (diff + spec + amb);
    
}