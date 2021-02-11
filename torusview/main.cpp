#include <iostream>
#include <map>
#include <cmath>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "stb_image_write.h"

#include "geometry.h"
#include "save_image.h"
#include "source_dir.h"
#include "shader.h"
#include "textures.h"
#include "camera.h"

const int WIDTH = 1280;
const int HEIGHT = 720;
const std::string TEXTUREFILE = "textures/activeTexture.png";
const bool WIREFRAME = false;
const bool DEPTH_TEST = true;
const bool BLEND = true;
const glm::vec4 AMBIENT_COLOR = glm::vec4(1.0f);
const std::string FILE_PATTERN = "screenshot-%.3i.png";

//- Camera object
//Camera camera(0, 3, 3.0,    0, 1, 0,    -90, 0);
Camera camera(0, 5, 5,    0, 1, 0,    -90, -45);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool spaceDown = false;
void processInput(GLFWwindow* window, float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!spaceDown)
            save_screenshot(FILE_PATTERN, window);
        spaceDown = true;
    } else spaceDown = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(CAMERA_FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(CAMERA_BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(CAMERA_LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(CAMERA_RIGHT, dt);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(CAMERA_UP, dt);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(CAMERA_DOWN, dt);
}

float lastMouseX, lastMouseY;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        firstMouse = false;
        lastMouseX = xpos;
        lastMouseY = ypos;
    }
    float xoffset = xpos - lastMouseX;
    float yoffset = ypos - lastMouseY;
    camera.ProcessMouseMovement(xoffset, yoffset);
    lastMouseX = xpos;
    lastMouseY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

void print(glm::vec3 vec) {
    std::cout << "(" << vec.x << " " << vec.y << " " << vec.z << ")\n";
}

int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window 
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "TorusView", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Load OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    if (DEPTH_TEST)
        glEnable(GL_DEPTH_TEST);

    // Generate shader program
    Shader myShader("shaders/shader.vs", "shaders/shader.fs");

    //- Projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), WIDTH/(float)HEIGHT, 
                                  0.1f, 100.0f);

    if (WIREFRAME)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (BLEND) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }


    /** Create the geometry for the torus **/

    unsigned int res = MESH_RESOLUTION;
    unsigned int n_verts = res*res;             // No. of vertices
    unsigned int n_tri = 2 * (res-1) * (res-1); // No. of triangles
    unsigned int n_idx = 3 * n_tri;             // No. of draw indices
    unsigned int NIDX = n_idx;
    // 3 spatial coords, 2 texture coords, 3 normal components per vertex
    unsigned int stride = 8;
    
    std::cout << "res: " << res << std::endl;
    std::cout << "n_tri: " << n_tri << std::endl;
    std::cout << "n_verts: " << n_verts << std::endl;
    std::cout << "n_idx: " << n_idx << std::endl;

    float vertices[n_verts * stride];
    unsigned int indices[n_idx];

    unsigned int VBO, VAO, EBO;
    makeGeometry(VBO, VAO, EBO, n_verts, n_idx, res, stride, vertices, indices);



    unsigned int texture = makeTexture(TEXTUREFILE, true);

    float pi = std::atan(1)*4;
    auto red = [&pi](float t) { return (std::sin(t/3)/2.0f) + 0.5f; };
    auto green = [&pi](float t) { return (std::sin(t/3+pi/3)/2.0f) + 0.5f; };
    auto blue = [&pi](float t) { return (std::sin(t/3+2*pi/3)/2.0f) + 0.5f; };
    auto alpha = [](float t) { return (std::sin(t)/2) + 0.5f; };

    myShader.use();
    myShader.setInt("texture1", 0);

    float oldT = glfwGetTime();

    // Render loop
    while (!glfwWindowShouldClose(window)) {

        float dt = glfwGetTime() - oldT;
        oldT = glfwGetTime();

        processInput(window, dt);

        projection = glm::perspective(glm::radians(camera.Zoom),
                            WIDTH/(float)HEIGHT, 0.1f, 100.0f);
        myShader.setMat4("projection", projection);

        glClearColor(AMBIENT_COLOR.r, AMBIENT_COLOR.g, AMBIENT_COLOR.b, AMBIENT_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float t = glfwGetTime();
        int alphaLocation = glGetUniformLocation(myShader.ID, "alpha");
        glUniform1f(alphaLocation, alpha(t));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glm::mat4 cameraView = camera.GetViewMatrix();

        //- Model and view matrices
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(90.0f),
        //                    glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view = cameraView;
        myShader.setMat4("model", model);
        myShader.setMat4("view", view);

        //glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void*)(NIDX/2 * sizeof(GLuint)));

        // Sort triangles by distance to camera
        using map_t = std::multimap<float, unsigned int>;
        map_t sorted; // <dist, idx_offset>
        for (unsigned int t = 0; t < n_tri; t++) {
            glm::vec3 center = glm::vec3(0.0f);
            glm::vec3 vert = glm::vec3(0.0f);
            for (unsigned int k = 0; k < 3; k++) {
                unsigned int i = indices[3*t+k];
                vert = glm::vec3(vertices[stride*i+0],
                                 vertices[stride*i+1],
                                 vertices[stride*i+2]);
                center += vert;
            }
            center /= 3.0f;
            float distance = glm::length(camera.Position - vert);
            sorted.insert(std::make_pair(distance, 3*t));
        }

        // Render each triangle in order
        for (map_t::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); it++) {
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 
                           (void*)(it->second * sizeof(GLuint)));
        }
        //glDrawElements(GL_TRIANGLES, NIDX, GL_UNSIGNED_INT, 0);

        // Check call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
