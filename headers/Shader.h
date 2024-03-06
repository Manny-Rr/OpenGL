#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {

public:
	//Reference ID
	unsigned int shader_ID;

	//constructor by using filepaths
	Shader(const char* vertex_path, const char* fragment_path);
	void use_shader() { glUseProgram(shader_ID); }
	void delete_shader() { glDeleteProgram(shader_ID); }
	void check_compile_status(unsigned int shader_reference, const char* name);
	void check_program_link_status(unsigned int program_reference);

	//uniform setters
	void set_vec3(const char* uniform_name, glm::vec3 source) {
		glUniform3fv(glGetUniformLocation(shader_ID, uniform_name), 1, glm::value_ptr(source));
	}
	void set_mat4(const char* uniform_name, glm::mat4 source) {
		glUniformMatrix4fv(glGetUniformLocation(shader_ID, uniform_name), 1, false, glm::value_ptr(source));
	}
	void set_float(const char* uniform_name, float value) {
		glUniform1f(glGetUniformLocation(shader_ID, uniform_name), value);
	}

	void set_texture(const char* texture_uniform_name, unsigned int texture_unit) {
		glUniform1i(glGetUniformLocation(shader_ID, texture_uniform_name), texture_unit);
	}
	void set_bool(const char* texture_uniform_name, bool flashlight_on) {
		glUniform1i(glGetUniformLocation(shader_ID, texture_uniform_name), flashlight_on);
	}


	
};





#endif // !SHADER_H



