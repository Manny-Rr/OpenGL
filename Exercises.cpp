
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include "headers/Shader.h"
#include "headers/Camera.h"
#include "headers/VAO.h"
#include "headers/Buffer.h"
#include "headers/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//here are all the typedefs
typedef GLFWwindow window;   //only to make window creation easier
typedef std::vector<float> f_vector;
typedef std::vector<unsigned int> ui_vector;

//forward declarations
void viewport_callback(window* window, int width, int height);
void process_input(window* window);
void mouse_input(window* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void update_delta_and_fps();

//global variables
window* window_one;

glm::vec3 light_source;
glm::vec3 light_position;

bool firstMouse = true;
Camera camera = Camera();
bool camera_disabled = true;

float delta_time = 0.0f;
float last_frame = 0.0f;
float fps = 0;
float time_elapsed = 0;




int main() {
    {
        /*
        * init the library an set its version
        * The first argument is a list of enums, basically telling the glfw which option we want to set and
        * then provide that argument as another enum or just a straight number
        * We also set the core porfile (the new modern one)
        */
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /*
        * Simplyfying the return value for writing sake
        * Just create window and check if it worked, if yes make it the current context
        */

        window_one = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangle", NULL, NULL);
        if (window_one == NULL) {
            std::cout << "Window creation failed" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window_one);

        //glad activation check
        //this must be done before using any OPENGL functions
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        //viewport callback
        glfwSetFramebufferSizeCallback(window_one, viewport_callback);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    }


    //keyboard and mouse callback
   // glfwSetKeyCallback(window_one, process_input);
    glfwSetCursorPosCallback(window_one, mouse_input);
    glfwSetScrollCallback(window_one, scroll_callback);

    //just flipping the axis
    //stbi_set_flip_vertically_on_load(true);


    Shader main_cube_shader = Shader("shaders/main_cube.vert", "shaders/main_cube.frag");
    Shader light_cube_shader = Shader("shaders/light_cube.vert", "shaders/light_source.frag");

    // ---------------------Texture code-------------------------

    //texture loading
    Texture diffuse_container("images/container2.png", GL_TEXTURE0, GL_RGBA);
    Texture specular_container("images/container2_specular.png", GL_TEXTURE1, GL_RGBA);

    //^^^^^^^^^^^^^^^^^^^^^^^Texture^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    main_cube_shader.use_shader();
    main_cube_shader.set_texture("material.diffuse_map", 0);
    main_cube_shader.set_texture("material.specular_map", 1);


    //f_vector vertices = {
    //    // positions          //colors          // texture coords
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right           0
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right        1
    //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left         2
    //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left            3
    //     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // back top right      4
    //     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   // back bottom right   5
    //    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   // back bottom left    6
    //    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f    // back top left       7
    //};

    f_vector vertices = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    ui_vector elems = {
        //front triangles
        0, 1, 3,
        1, 2 ,3,

        //back triangles
        4, 5, 7,
        5, 6 ,7,

        //top triangles
        0, 4, 7,
        0, 3, 7,

        //bottom triangles
        1, 5, 6,
        1, 2, 6,

        //right
        0, 1, 5,
        0, 5, 4,

        //left
        3, 2, 6,
        3, 7, 6


    };


    //order: Create & Bind VAO -> Bind & Buffer any required Objects -> Enable the vertex attribute pointer configuration -> Unbind VAO then bind when required
    VAO main_cube = VAO();
    main_cube.bind_VAO();
    Buffer vbo = Buffer(vertices);
    Buffer ebo = Buffer(elems);
    //Buffer::config_vertex_attrib(0, 3, 8, 0);   //positions
    //Buffer::config_vertex_attrib(1, 3, 8, 3);   //colors
    //Buffer::config_vertex_attrib(2, 2, 8, 6);   //texture coords
    //unbind VAO
    Buffer::config_vertex_attrib(0, 3, 8, 0);   //positions
    Buffer::config_vertex_attrib(1, 3, 8, 3);   //normal
    Buffer::config_vertex_attrib(2, 2, 8, 6);   //texture coordinates

    main_cube.unbind_VAO();



    VAO light_cube = VAO();
    light_cube.bind_VAO();
    Buffer vbo_two = Buffer(vertices);
    Buffer ebo_two = Buffer(elems);
    Buffer::config_vertex_attrib(0, 3, 8, 0);   //positions
    //Buffer::config_vertex_attrib(1, 3, 8, 3);   //colors
    //Buffer::config_vertex_attrib(2, 2, 8, 6);   //texture coords
    //unbind VAO
    light_cube.unbind_VAO();


    //^^^^^^^^^^^^^^includes texture, shader, buffer code above ^^^^^^^^^^^^^^^^

    /*Object coordinates transform order:
      Local Space coords -> World Space coords (done using the model matrix which translates, scales, rotates object to where they belong ->
      -> world coords are turned into view space using a view matrix ->
      -> view to clip space using projection matrix whether orthogonal or perspective ->
      -> after this, perpective division takes place, which simply normalises x,y,z vector using the w value (does not happen for orthogonal) ->
      -> w values becomes higher the further the object gets

        final trnsform matrix = projection * view * model * location_vector (read from right to left)
     */

     //-------------------------------camera code---------------------------------
     //model matrix
    glm::mat4 model(1.0f);
    //model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = camera.get_view_matrix();

    //perpective projection
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / SCR_HEIGHT, 0.01f, 100.0f);

    glm::mat4 pre_model_transform = projection * view;

    //main_cube_shader.use_shader();
    //glUniformMatrix4fv(glGetUniformLocation(main_cube_shader.shader_ID, "final_transform"), 1, false, glm::value_ptr(pre_model_transform));

    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^camera code^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


    glEnable(GL_DEPTH_TEST);
    if (!camera_disabled)
        glfwSetInputMode(window_one, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    float color_clear[] = { 0.5f, 0.5f, 0.5f };

    light_source = glm::vec3(1.0f);
    light_position = glm::vec3(1.2f, 0.5f, 2.0f);





    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window_one, true);
    ImGui_ImplOpenGL3_Init("#version 330");



    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    while (!glfwWindowShouldClose(window_one)) {

        process_input(window_one);
        update_delta_and_fps();
        glClearColor(color_clear[0], color_clear[1], color_clear[2], 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        view = camera.get_view_matrix();
        projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / SCR_HEIGHT, 0.01f, 100.0f);

        //object cube
        main_cube_shader.use_shader();
        main_cube.bind_VAO();
        model = glm::rotate(model, glm::radians(0.005f), glm::vec3(0.0f, 1.0f, 0.0f));

        main_cube_shader.set_vec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        main_cube_shader.set_vec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        main_cube_shader.set_vec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        main_cube_shader.set_float("material.shininess", 128.0f);

        //directional light
        main_cube_shader.set_vec3("direct_light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        main_cube_shader.set_vec3("direct_light.ambient", glm::vec3(0.2f));
        main_cube_shader.set_vec3("direct_light.diffuse", glm::vec3(1.0f));
        main_cube_shader.set_vec3("direct_light.specular", glm::vec3(1.0f));

        //point light
        main_cube_shader.set_vec3("point_light.position", light_position);
        main_cube_shader.set_float("point_light.constant", 1.0f);
        main_cube_shader.set_float("point_light.linear", 0.07f);
        main_cube_shader.set_float("point_light.quadratic", 0.017f);
        main_cube_shader.set_vec3("point_light.ambient", glm::vec3(0.2f));
        main_cube_shader.set_vec3("point_light.diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
        main_cube_shader.set_vec3("point_light.specular", glm::vec3(0.0f, 1.0f, 0.0f));


        //spot light
        main_cube_shader.set_vec3("spot_light.light_front", camera.camera_direction);
        main_cube_shader.set_float("spot_light.inner_cutoff", glm::cos(glm::radians(12.5f)));
        main_cube_shader.set_float("spot_light.outer_cutoff", glm::cos(glm::radians(20.0f)));
        main_cube_shader.set_vec3("spot_light.ambient", glm::vec3(0.2f));
        main_cube_shader.set_vec3("spot_light.diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
        main_cube_shader.set_vec3("spot_light.specular", glm::vec3(1.0f, 0.0f, 0.0f));

        main_cube_shader.set_vec3("camera_position", camera.camera_position);
        main_cube_shader.set_mat4("projection", projection);
        main_cube_shader.set_mat4("view", view);
        main_cube_shader.set_mat4("model", model);

        //glDrawElements(GL_TRIANGLES, elems.size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //light cube
        light_cube_shader.use_shader();
        light_cube.bind_VAO();
        glm::mat4 light_model = glm::mat4(1.0f);
        light_model = glm::translate(light_model, light_position);
        light_model = glm::scale(light_model, glm::vec3(0.2f));
        main_cube_shader.set_mat4("projection", projection);
        main_cube_shader.set_mat4("view", view);
        main_cube_shader.set_mat4("model", light_model);
        //glDrawElements(GL_TRIANGLES, elems.size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);


        ImGui::Begin("This is the window");
        ImGui::Text("AYAYAYA");
        ImGui::ColorEdit3("Background", color_clear);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_one);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //remember to eventually add clean-up code
    //glDeleteVertexArrays(1, &vao);
    //glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;

}



void process_input(window* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.keyboard_input(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.keyboard_input(BACK, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.keyboard_input(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.keyboard_input(RIGHT, delta_time);


    //light source movement for testing
    if (glfwGetKey(window, GLFW_KEY_I))
        light_position.z -= 0.0025f;
    if (glfwGetKey(window, GLFW_KEY_K))
        light_position.z += 0.0025f;
    if (glfwGetKey(window, GLFW_KEY_J))
        light_position.x -= 0.0025f;
    if (glfwGetKey(window, GLFW_KEY_L))
        light_position.x += 0.0025f;

    if (glfwGetKey(window, GLFW_KEY_C))
    {
        camera_disabled = !camera_disabled;

        if (camera_disabled)
            glfwSetInputMode(window_one, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window_one, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }




}

void update_delta_and_fps() {
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    time_elapsed += delta_time;
    fps++;
    if (time_elapsed >= 1) {
        glfwSetWindowTitle(window_one, std::to_string(fps).c_str());
        fps = 0;
        time_elapsed = 0;
    }
}

void mouse_input(window* window, double xposIn, double yposIn) {

    if (camera_disabled)
        return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.mouse_input(xoffset, yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.scroll_zoom((float)yoffset);
}

void viewport_callback(window* window, int width, int height) {
    glViewport(0, 0, width, height);
}