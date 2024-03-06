#ifndef TEXTURE_H
#define TEXTTURE_H

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <iostream>


class Texture {

public:
    unsigned int texture_ID;

    int width, height, channels;

    Texture(const char* file_path, GLenum texture_unit, GLenum color_format) {

        glGenTextures(1, &texture_ID);
        glActiveTexture(texture_unit);
        glBindTexture(GL_TEXTURE_2D, texture_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      
        //load the data into the 2D buffer
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, color_format, GL_UNSIGNED_BYTE, load_image(file_path));
        glGenerateMipmap(GL_TEXTURE_2D);

    }


private:

    unsigned char* load_image(const char* file_path) {

        unsigned char* image_data = stbi_load(file_path, &width, &height, &channels, 0);
        if (!image_data)
            std::cout << "failure loading image data\0";
        return image_data;
    }


};


#endif // !TEXTURE_H
