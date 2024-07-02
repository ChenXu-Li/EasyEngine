#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ScreenToModel{
public:
    static glm::vec3 screenToNDC(int x, int y, float z_buffer, int screenWidth, int screenHeight) {
        float x_ndc = (2.0f * x) / screenWidth - 1.0f;
        float y_ndc = 1.0f - (2.0f * y) / screenHeight;
        float z_ndc = 2.0f * z_buffer - 1.0f;
        return glm::vec3(x_ndc, y_ndc, z_ndc);
    }

    // Constructs clip coordinates from NDC coordinates
    static glm::vec4 ndcToClip(const glm::vec3& ndcCoords) {
        return glm::vec4(ndcCoords, 1.0f);
    }

    // Transforms clip coordinates to view space coordinates
    static glm::vec4 clipToView(const glm::vec4& clipCoords, const glm::mat4& projectionMatrix) {
        glm::mat4 invProjectionMatrix = glm::inverse(projectionMatrix);
        glm::vec4 viewCoords = invProjectionMatrix * clipCoords;
        viewCoords /= viewCoords.w;  // Perspective division
        return viewCoords;
    }

    // Transforms view space coordinates to world space coordinates
    static glm::vec3 viewToWorld(const glm::vec4& viewCoords, const glm::mat4& viewMatrix) {
        glm::mat4 invViewMatrix = glm::inverse(viewMatrix);
        glm::vec4 worldCoords = invViewMatrix * viewCoords;
        return glm::vec3(worldCoords);
    }
    static glm::vec3 worldToModel(const glm::vec3& worldCoords, const glm::mat4& modelMatrix) {
        glm::mat4 invModelMatrix = glm::inverse(modelMatrix);
        glm::vec4 modelCoords = invModelMatrix * glm::vec4(worldCoords, 1.0f);
        return glm::vec3(modelCoords);
    }
    static glm::vec3 screenToWorld(int x, int y, float z_buffer, int screenWidth, int screenHeight,
        const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
        glm::vec3 ndcCoords = screenToNDC(x, y, z_buffer, screenWidth, screenHeight);
        glm::vec4 clipCoords = ndcToClip(ndcCoords);
        glm::vec4 viewCoords = clipToView(clipCoords, projectionMatrix);      
        return   viewToWorld(viewCoords, viewMatrix);
    }
    // Transforms screen coordinates to model space coordinates
    static glm::vec3 screenToModel(int x, int y, float z_buffer, int screenWidth, int screenHeight,
        const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::mat4& modelMatrix) {
        glm::vec3 ndcCoords = screenToNDC(x, y, z_buffer, screenWidth, screenHeight);
        glm::vec4 clipCoords = ndcToClip(ndcCoords);
        glm::vec4 viewCoords = clipToView(clipCoords, projectionMatrix);
        glm::vec3 worldCoords = viewToWorld(viewCoords, viewMatrix);
        return worldToModel(worldCoords, modelMatrix);
    }
    
};
class ModelToScreen {
public:
    // Transforms model space coordinates to world space coordinates
    static glm::vec4 modelToWorld(const glm::vec3& modelCoords, const glm::mat4& modelMatrix) {
        return modelMatrix * glm::vec4(modelCoords, 1.0f);
    }

    // Transforms world space coordinates to view space coordinates
    static glm::vec4 worldToView(const glm::vec4& worldCoords, const glm::mat4& viewMatrix) {
        return viewMatrix * worldCoords;
    }

    // Transforms view space coordinates to clip space coordinates
    static glm::vec4 viewToClip(const glm::vec4& viewCoords, const glm::mat4& projectionMatrix) {
        return projectionMatrix * viewCoords;
    }

    // Converts clip coordinates to normalized device coordinates (NDC)
    static glm::vec3 clipToNDC(const glm::vec4& clipCoords) {
        glm::vec3 ndcCoords;
        ndcCoords.x = clipCoords.x / clipCoords.w;
        ndcCoords.y = clipCoords.y / clipCoords.w;
        ndcCoords.z = clipCoords.z / clipCoords.w;
        return ndcCoords;
    }

    // Converts normalized device coordinates (NDC) to screen coordinates
    static glm::vec3 ndcToScreen(const glm::vec3& ndcCoords, int screenWidth, int screenHeight) {
        glm::vec3 screenCoords;
        screenCoords.x = (ndcCoords.x + 1.0f) / 2.0f * screenWidth;
        screenCoords.y = (1.0f - ndcCoords.y) / 2.0f * screenHeight;
        screenCoords.z = (ndcCoords.z + 1.0f) / 2.0f; // Depth buffer value
        return screenCoords;
    }

    // Transforms model space coordinates to screen coordinates
    static glm::vec3 modelToScreen(const glm::vec3& modelCoords, int screenWidth, int screenHeight,
        const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
        glm::vec4 worldCoords = modelToWorld(modelCoords, modelMatrix);
        glm::vec4 viewCoords = worldToView(worldCoords, viewMatrix);
        glm::vec4 clipCoords = viewToClip(viewCoords, projectionMatrix);
        glm::vec3 ndcCoords = clipToNDC(clipCoords);
        return ndcToScreen(ndcCoords, screenWidth, screenHeight);
    }
};



