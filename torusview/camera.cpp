#include "camera.h"

Camera::Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = YAW, float pitch = PITCH)
               : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
                 MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY),
                 Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ,
               float upX, float upY, float upZ, 
               float yaw, float pitch)
               : Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ),
                        yaw, pitch)
{  }

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    //TODO proper handling of pitch limiting

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();

    float velocity = MovementSpeed * deltaTime;
    glm::mat4 transform = glm::mat4(1.0f);
    if (direction == CAMERA_RIGHT)
        transform = glm::rotate(transform, -velocity, WorldUp);
    if (direction == CAMERA_LEFT)
        transform = glm::rotate(transform, velocity, WorldUp);
    if (direction == CAMERA_FORWARD && Pitch > -85)
        transform = glm::rotate(transform, velocity, Right);
    if (direction == CAMERA_BACKWARD && Pitch < 85)
        transform = glm::rotate(transform, -velocity, Right);
    if (direction == CAMERA_UP && velocity < glm::length(Position))
        Position += Front * velocity;
    if (direction == CAMERA_DOWN)
        Position -= Front * velocity;

    Position = glm::vec4(Position, 1.0f) * transform;
    LookAt(glm::vec3(0.0f));
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset,
                                  bool constrainPitch)
{
    /*
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch -= yoffset;

        // make sure that when pitch is out of bounds
        //  so screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
        //LookAt(glm::vec3(0.0f));
    */
}

void Camera::ProcessMouseScroll(float yoffset) {
    float velocity = yoffset / 2.0f;
    if (velocity < glm::length(Position))
        Position += Front * yoffset/2.0f;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::LookAt(glm::vec3 target) {
    Front = glm::normalize(target - Position);
    Pitch   = glm::degrees(glm::asin(Front.y));
    Yaw = glm::degrees(std::atan2(Front.z, Front.x));

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}
