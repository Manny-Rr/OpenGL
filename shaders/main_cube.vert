
#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture_coord;
  
out vec3 Normal;
out vec2 texture_coords;
out vec3 frag_position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    frag_position = vec3(model * vec4(aPos, 1.0));
    texture_coords = aTexture_coord;
} 

