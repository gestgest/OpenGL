#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emission;  // 새로 추가: 발광 컴포넌트
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;  
in vec2 TexCoord;
in vec3 FragPos;  

// texture samplers
uniform sampler2D texture1;

// light, material 
uniform Material material;
uniform Light light;
uniform vec3 eyePos;

void main()
{
    //diffuse term
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (texture(texture1, TexCoord).rgb);
    //vec3 diffuse = light.diffuse * (diff * material.diffuse);
    //vec3 diffuse = light.diffuse * (diff * texture(texture1, TexCoord).rgb);

	// specular term 
    vec3 View = normalize(eyePos - FragPos);
	vec3 refl = 2.0 * norm * dot(norm, lightDir) - lightDir; //    vec3 reflectDir = reflect(-lightDir, norm);  
 	float spec = pow(max(dot(refl, View), 0.0), material.shininess);  //계산 식
	vec3 specular = light.specular * (spec * material.specular);

	// ambient term s 외적 m
     vec3 ambient = light.ambient * material.ambient;
            
    // 수정: emission 추가 (태양만 밝게)
    vec3 result = (ambient + diffuse + specular) + material.emission;
    //vec3 result = diffuse;

    FragColor = vec4(result, 1.0);
} 
