#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>


//CAMERA CLASS HANDLES THE VIEW MATRIX

enum Camera_Movement {
    FORWARD,
    BACK,
    RIGHT,
    LEFT
};



const float movement_speed = 0.1f;
const float sensitivity = 0.15f; // change this value to your liking
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 400.0f;
float lastY = 300.0f;
float FOV = 45.0f;


class Camera {

public:
    float fov;

	glm::vec3 camera_position;
	glm::vec3 camera_direction;
	glm::vec3 camera_up;
    glm::vec3 camera_sideways;  //cross product of direction & up vectors, basically gives the x-axis relative to position & orientation

    Camera(
        glm::vec3 position  = glm::vec3(0.0f, 0.0f, 5.0f),      //camera position
        glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f)       //up vector
        ) 
        : camera_direction(glm::vec3(0.0f, 0.0f, -1.0f)) /*default camera direction*/,
          fov(FOV)
    {
        camera_position = position;
        camera_up = up_vector;
        camera_sideways = glm::normalize(glm::cross(camera_direction, camera_up));
    }

    //THIS IS THE KEY FUNCTION TO BE CALLED :::: IT RETURNS THE VIEW MATRIX

    glm::mat4 get_view_matrix() {
        return glm::lookAt(camera_position, camera_position + camera_direction, camera_up);
    }

    
    void mouse_input(float xoffset, float yoffset) {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (yaw > 359.0f || yaw < -359.0f)
            yaw = 0.0f;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        update_vectors();
        
    }

    void keyboard_input(Camera_Movement direction, float delta_time) {

        float cameraSpeed = movement_speed * delta_time * 100.0f;

        if (direction == FORWARD)
            camera_position += cameraSpeed * camera_direction;
        if (direction == BACK)
            camera_position -= cameraSpeed * camera_direction;
        if (direction == RIGHT)
            camera_position += cameraSpeed * camera_sideways;
        if (direction == LEFT)
            camera_position -= cameraSpeed * camera_sideways;
    }

    void scroll_zoom(float zoom) {
        fov -= zoom;
        if (fov > 80.0f)
            fov = 80.0f;
        if (fov < 45.0f)
            fov = 45.0f;
    }


private:
    void update_vectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera_direction = glm::normalize(front);

        //update the sideways vector after
        camera_sideways = glm::normalize(glm::cross(camera_direction, camera_up));
    }

};


#endif //CAMERA_H