#include "headers/Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader (const char* vertex_path, const char* fragment_path) {

	//create const char* object for opengl functions
	const char* vertex_code;
	const char* fragment_code;
	//create string objects for reading into a buffer
	std::string vertex_string;
	std::string fragment_string;
	//stringstream objects
	std::stringstream vertex_string_stream, fragment_string_stream;
	//create ifstream object for opening and reading
	std::ifstream vertex_stream;
	std::ifstream fragment_stream;


	vertex_stream.open(vertex_path);
	fragment_stream.open(fragment_path);

	vertex_string_stream << vertex_stream.rdbuf();
	fragment_string_stream << fragment_stream.rdbuf();

	vertex_string = vertex_string_stream.str();
	fragment_string = fragment_string_stream.str();


	vertex_code = vertex_string.c_str();
	fragment_code = fragment_string.c_str();

	//std::cout << vertex_code << "\n\n\n" << fragment_code << std::endl;

	// ^^^^^^^^^^^^^^^^ everything above handles opening and reading from the given filepath ^^^^^^^^^^^^^^

	unsigned int vertex_shader, fragment_shader;

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_code, NULL);
	glCompileShader(vertex_shader); 

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_code, NULL);
	glCompileShader(fragment_shader);

	//shader success check
	check_compile_status(vertex_shader, "vertex");
	check_compile_status(fragment_shader, "fragment");

	//ID reference that will used to use or delete the program
	shader_ID = glCreateProgram();
	glAttachShader(shader_ID, vertex_shader);
	glAttachShader(shader_ID, fragment_shader);
	glLinkProgram(shader_ID);

	check_program_link_status(shader_ID);

	//delete
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

}

void Shader::check_compile_status(unsigned int shader_reference, const char* name) {
	int success;
	glGetShaderiv(shader_reference, GL_COMPILE_STATUS, &success);
	char infoLog[512];
	if (!success)
	{
		glGetShaderInfoLog(shader_reference, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << name << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::check_program_link_status(unsigned int program_reference) {
	int success;
	char infoLog[512];
	glGetProgramiv(program_reference, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program_reference, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}




