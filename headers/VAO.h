#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <vector>


class VAO {

private:

public:	

	unsigned int ID;

	VAO(unsigned int number_of_vaos = 1) {
		glGenVertexArrays(number_of_vaos, &ID);
	}

	void bind_VAO() {
		glBindVertexArray(ID);
	}
	void unbind_VAO() {
		glBindVertexArray(0);
	}
	









	
	

	







};











#endif //VAO_H
