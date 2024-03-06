#version 330 core

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;
    float shininess;
};

struct Directional_Light {
    //for the world directional light
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
};

struct Point_Light{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    //each light can have its own color
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spot_Light{
    vec3 light_front;  //camera front vector
    float inner_cutoff;
    float outer_cutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};



//functions
vec3 get_directional_light(Directional_Light light, vec3 norm, vec3 view_direction, vec3 tex_color, vec3 spec_color);
vec3 get_point_light(Point_Light light, vec3 norm, vec3 frag_pos, vec3 view_direction, vec3 tex_color, vec3 spec_color);
vec3 get_spot_light(Spot_Light light, vec3 norm, vec3 frag_pos, vec3 view_direction, vec3 tex_color, vec3 spec_color);

out vec4 FragColor;  

in vec3 Normal;
in vec2 texture_coords;
in vec3 frag_position;


uniform vec3 camera_position;
uniform bool flashlight_on;


uniform Material material;
uniform Directional_Light direct_light;    //will need to change this variable name
uniform Point_Light point_light;
uniform Spot_Light spot_light;


//multiple lights
#define NUM_OF_LIGHTS 4
uniform Point_Light multi_lights[NUM_OF_LIGHTS];



void main()
{
    //accessed by all calcs
    vec3 norm = normalize(Normal);
    vec3 view_direction = normalize(camera_position - frag_position);
    //these are used by each lighting type so pass them in as arguments other than spot light for now
    vec3 tex_color = vec3(texture(material.diffuse_map, texture_coords));
    vec3 spec_color = vec3(texture(material.specular_map, texture_coords));

    vec3 final_color = vec3(0.0f);
    
    //directional light
    final_color += get_directional_light(direct_light, norm, view_direction, tex_color, spec_color);

    //point light(s)
    final_color += get_point_light(point_light, norm, frag_position, view_direction, tex_color, spec_color);

    for (int i = 0; i < NUM_OF_LIGHTS; i++){
        final_color += get_point_light(multi_lights[i], norm, frag_position, view_direction, tex_color, spec_color);
    }


    //spotlight 
    if (flashlight_on)
        final_color += get_spot_light(spot_light, norm, frag_position, view_direction, tex_color, spec_color);

      
    FragColor = vec4(final_color, 1.0);

}

vec3 get_directional_light(Directional_Light light, vec3 norm, vec3 view_direction, vec3 tex_color, vec3 spec_color){
   
    //original -> from light towards fragment   so   inverse it new_direction -> fragment to light
    vec3 light_direction = normalize(-direct_light.direction);
    
    //ambient 
    vec3 ambient = direct_light.ambient * tex_color;

    //diffuse
    float diffuse_value = max(dot(norm, light_direction), 0);
    vec3 diffuse = diffuse_value * direct_light.diffuse * tex_color;
   
    //specular
    vec3 reflection_vector = reflect(-light_direction, norm);
    float spec_value = pow(max(dot(view_direction, reflection_vector), 0), material.shininess);
    vec3 specular = spec_value * direct_light.specular * spec_color;

   return (ambient + diffuse + specular);

}

vec3 get_point_light(Point_Light light, vec3 norm, vec3 frag_pos, vec3 view_direction, vec3 tex_color, vec3 spec_color){

    //get light direction first from the fragment
    vec3 light_direction = normalize(light.position - frag_pos);

    //ambient 
    vec3 ambient = light.ambient * tex_color;

    //diffuse
    float diffuse_value = max(dot(norm, light_direction), 0);
    vec3 diffuse = diffuse_value * light.diffuse * tex_color;

    //specular
    vec3 reflection_vector = reflect(-light_direction, norm);
    float spec_value = pow(max(dot(view_direction, reflection_vector), 0), material.shininess);
    vec3 specular = spec_value * light.specular * spec_color;

    float light_distance = length(light.position - frag_position);
    float attentuation = 1 / (light.constant + (light.linear *  light_distance) + light.quadratic * (light_distance * light_distance));

    ambient *- attentuation;
    diffuse *= attentuation;
    specular *= attentuation;

    return (ambient + diffuse + specular);

}

vec3 get_spot_light(Spot_Light light, vec3 norm, vec3 frag_pos, vec3 view_direction, vec3 tex_color, vec3 spec_color){
    
    //inverse the camera front vector
    vec3 light_direction = normalize(-light.light_front);

    //ambient
    vec3 ambient = light.ambient * tex_color;

    //diffuse
    float diffuse_value = max(dot(norm, light_direction), 0);
    vec3 diffuse = diffuse_value * light.diffuse * tex_color;

    //specular
    vec3 reflection_vector = reflect(-light_direction, norm);
    float spec_value = pow(max(dot(view_direction, reflection_vector), 0), material.shininess);
    vec3 specular = spec_value * light.specular * spec_color;


    //for spotlight 
    float theta = dot(view_direction, normalize(-light.light_front));
    float epsilon = light.inner_cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0f, 1.0f);

    //ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);

}