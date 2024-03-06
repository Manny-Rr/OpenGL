#ifndef  VBO_H
#define VBO_H
#include <vector>
#include <glad/glad.h>
class Buffer {

public:

	unsigned int ID;

	//for VBO
	Buffer(std::vector <float>& vertices) {

		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	}

	//for EBO
	Buffer(std::vector <unsigned int>& elems) {	
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elems.size() * sizeof(unsigned int), elems.data(), GL_STATIC_DRAW);
	}

	static void config_vertex_attrib(unsigned int layout_location, int size, unsigned int stride, unsigned int offset) {

		glVertexAttribPointer(layout_location, size, GL_FLOAT, false, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(layout_location);

	}





};




#endif // ! VBO_H

