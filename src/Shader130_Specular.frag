#version 130
/* ---------- Removed by Calvin.  2014-12-18 ----------
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
*/

/* Note: because we now use a material struct again you want to change your
mesh class to bind all the textures using material.texture_diffuseN instead of
texture_diffuseN. */

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NUMBER_POINT_LIGHTS 2   // Assign two point lights

in vec3 fragPosition;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform PointLight pointLights[NUMBER_POINT_LIGHTS];
//uniform Material material;            // Removed by Calvin.  2014-12-18
uniform sampler2D texture_diffuse1;     // Added by Calvin.  2014-12-18
uniform sampler2D texture_specular1;    // Added by Calvin.  2014-12-18

// Function prototypes
//vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir);                                         // Removed by Calvin.  2014-12-18
vec3 CalcPointLight(PointLight light, sampler2D texture_diffuse, sampler2D texture_specular, vec3 normal, vec3 fragPos, vec3 viewDir);  // Added by Calvin.  2014-12-18

void main()
{
    vec3 result = vec3(0.0f);
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 norm = normalize(Normal);

    /* ---------- This For-Loop might cause textures with noises (don't know how it happens), and needs to be removed. ----------
    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
        //result += CalcPointLight(pointLights[i], material, norm, fragPosition, viewDir);                          // Removed by Calvin.  2014-12-18
        result += CalcPointLight(pointLights[i], texture_diffuse1, texture_specular1, norm, fragPosition, viewDir); // Added by Calvin.  2014-12-18
    */
    /*
    // ---------- Instead, without using For-Loop, below statements do work perfactly. ----------
    result  = CalcPointLight(pointLights[0], texture_diffuse1, texture_specular1, norm, fragPosition, viewDir);     // Added by Calvin.  2014-12-18
    result += CalcPointLight(pointLights[1], texture_diffuse1, texture_specular1, norm, fragPosition, viewDir);     // Added by Calvin.  2014-12-18
*/
    for (int i = 0; i < NUMBER_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], texture_diffuse1, texture_specular1, norm, fragPosition, viewDir); // Added by Calvin.  2014-12-18

    //color = vec4(result*2, 1.0f);
    color = vec4(result, 1.0f);

    // ---------- Using below statement will by pass all the massive calculation, and just output simple texture only. ----------
    //color = vec4(texture(texture_diffuse1, TexCoords)*2);       // Added by Calvin. 2014-12-17
}


// Calculates the color when using a point light.
//vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir)                                          // Removed by Calvin.  2014-12-18
vec3 CalcPointLight(PointLight light, sampler2D texture_diffuse, sampler2D texture_specular, vec3 normal, vec3 fragPos, vec3 viewDir)   // Added by Calvin.  2014-12-18
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);  // Removed by Calvin.  2014-12-18
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);               // Added by Calvin.  2014-12-18
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32) * sign(diff);  // Modified by Calvin.  2015-08-31
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // Combine results
    /* ---------- Removed by Calvin.  2014-12-18 ----------
    vec3 ambient = light.ambient * vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(mat.texture_specular1, TexCoords));
    */
    // ---------- Modified by Calvin.  2014-12-18 ----------
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse, TexCoords))*4;
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular, TexCoords));
    // -----------------------------------------------------

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //return (ambient);
    //return (diffuse);
    //return (specular);
    //return (ambient + diffuse);
    //return (ambient + specular);
    return (ambient + diffuse + specular);
}


