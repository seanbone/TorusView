#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

/**
 * Defines several options for camera movement.
 * Used as abstraction to stay away from window-system specific input methods.
 */
enum Camera_Movement {
    CAMERA_FORWARD,
    CAMERA_BACKWARD,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    CAMERA_UP,
    CAMERA_DOWN
};

/**
 * Default camera values-
 */
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  3.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

/**
 * An abstract camera class that processes input and calculates 
 *  the corresponding Euler Angles, Vectors and Matrices for use in OpenGL.
 */
class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    /**
     * Constructor with vectors.
     */
    Camera(glm::vec3 Position, glm::vec3 up, float yaw, float pitch);

    /**
     * Constructor with scalars.
     */
    Camera(float posX, float posY, float posZ,
           float upX, float upY, float upZ,
           float yaw, float pitch);

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    void ProcessMouseMovement(float xoffset, float yoffset, 
                              bool constrainPitch = true);

    void ProcessMouseScroll(float yoffset);

    /**
     * Rotate camera to look at a point.
     */
    void LookAt(glm::vec3 target);

private:
    /**
     * Updates camera vectors based on pitch and yaw.
     */
    void updateCameraVectors();
};

#endif
