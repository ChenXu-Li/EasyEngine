#include "Camera.h"
#include<iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
extern bool keys[1024];
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
{
    
    this->Position = position;
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->MovementSpeed = SPEED;
    this->RotateSpeed = ROTATESPEED;
    this->MouseSensitivity = SENSITIVTY;
    this->Zoom = ZOOM;
    FlagAngleChanged = true;
    //this->updateCameraVectors();
    std::cout << " [create camera]" << std::endl;
    std::cout << " [camera position]" << this->Position[0] << "   " << this->Position[1] << "   " << this->Position[2] << std::endl;
    std::cout << " [camera rotation]" << this->Yaw << "   " << this->Pitch <<  std::endl;
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
{
    this->Position = glm::vec3(posX, posY, posZ);
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    if (FlagAngleChanged) {
        updateCameraVectors();
        FlagAngleChanged = false;
    }
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void Camera::OnUpdate(float deltaTime)
{
    Do_Movement(deltaTime);
}

void Camera::OnRender()
{
}

void Camera::OnImGuiRender()
{
    if (ImGui::TreeNode("Camera Arg")) {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Camera Position"))
        {
            ImGui::BeginGroup();
            ImGui::PushItemWidth(80);
            ImGui::DragFloat("X", &Position.x, 0.1f);
            ImGui::SameLine();
            ImGui::DragFloat("Y", &Position.y, 0.1f);
            ImGui::SameLine();
            ImGui::DragFloat("Z", &Position.z, 0.1f);
            ImGui::PopItemWidth();
            ImGui::EndGroup();

            ImGui::TreePop();
        }

        // 显示相机角度
        if (ImGui::TreeNode("Camera Angle"))
        {
            static bool yawChanged = false;
            static bool pitchChanged = false;
            ImGui::BeginGroup();
            ImGui::PushItemWidth(80);
            if (ImGui::DragFloat("Yaw", &Yaw, 0.1f, -180.0f, 180.0f))
            {
                yawChanged = true;
            }
            ImGui::SameLine();
            if (ImGui::DragFloat("Pitch", &Pitch, 0.1f, -89.0f, 89.0f))
            {
                pitchChanged = true;
            }

            if (yawChanged || pitchChanged)
            {
                FlagAngleChanged = true;
                yawChanged = false;
                pitchChanged = false;
            }
            ImGui::PopItemWidth();
            ImGui::EndGroup();
            ImGui::TreePop();
        }

        // 显示相机速度
        if (ImGui::TreeNode("Camera Speed"))
        {
            ImGui::DragFloat("Movement Speed", &MovementSpeed, 0.1f, 0.0f, 20.0f);
            ImGui::DragFloat("Rotation Speed", &RotateSpeed, 0.1f, 0.0f, 60.0f);
            ImGui::TreePop();
        }

        // 显示相机缩放
        if (ImGui::TreeNode("Camera Zoom"))
        {
            ImGui::DragFloat("Zoom", &Zoom, 0.1f, 1.0f, 45.0f);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }


}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD) {
        this->Position += this->Front * velocity;
    }      
    if (direction == BACKWARD) {
        this->Position -= this->Front * velocity;
    }      
    if (direction == LEFT) {
        this->Position -= this->Right * velocity;
    }      
    if (direction == RIGHT) {
        this->Position += this->Right * velocity;
    }
    if (direction == UP) {
        this->Position += this->WorldUp * velocity;
    }
    if (direction == DOWN) {
        this->Position -= this->WorldUp * velocity;
    }
    velocity =  this->RotateSpeed * deltaTime;
    if (direction == LEFTROTATE) {
        this->Yaw -= velocity;
        FlagAngleChanged = true;
    }
    if (direction == RIGHTROTATE) {
        this->Yaw += velocity;
        FlagAngleChanged = true;
    }
    if (direction == UPROTATE) {
        this->Pitch += velocity;
        FlagAngleChanged = true;
    }
    if (direction == DOWNROTATE) {
        this->Pitch -= velocity;
        FlagAngleChanged = true;
    }
    if (true)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }
    
}
        

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}

void Camera::Do_Movement(float deltaTime)
{
        // Camera controls
        if (keys[GLFW_KEY_W]) {
            ProcessKeyboard(FORWARD, deltaTime);
        }
        if (keys[GLFW_KEY_S]) {
            ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (keys[GLFW_KEY_A]) {
            ProcessKeyboard(LEFT, deltaTime);
        }
        if (keys[GLFW_KEY_D]) {
            ProcessKeyboard(RIGHT, deltaTime);
        }
        if (keys[GLFW_KEY_Q]) {
            ProcessKeyboard(DOWN, deltaTime);
        }
        if (keys[GLFW_KEY_E]) {
            ProcessKeyboard(UP, deltaTime);
        }
        if (keys[GLFW_KEY_J]) {
            ProcessKeyboard(LEFTROTATE, deltaTime);
        }
        if (keys[GLFW_KEY_L]) {
            ProcessKeyboard(RIGHTROTATE, deltaTime);
        }
        if (keys[GLFW_KEY_I]) {
            ProcessKeyboard(UPROTATE, deltaTime);
        }
        if (keys[GLFW_KEY_K]) {
            ProcessKeyboard(DOWNROTATE, deltaTime);
        }


}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up = glm::normalize(glm::cross(this->Right, this->Front)); 
}
