#pragma once

#include <vector>

// 通用模板类，用于存储顶点和索引数据
template<typename T>
struct MeshData {
    static std::vector<float> GetVertices();
    static std::vector<unsigned int> GetIndices();
};

// 特化模板用于 Cube
template<>
struct MeshData<class Cube> {
    static std::vector<float> GetVertices() {
        return {
            // Positions           // Colors            // Texture Coords
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
             0.0f,  0.0f, 0.5f,  1.0f, 1.0f, 1.0f,   0.5f, 0.5f,
        };
    }

    static std::vector<unsigned int> GetIndices() {
        return {
            0, 1, 2,
            2, 3, 0,
            0, 4, 3,
            0, 1, 4,
            1, 2, 4,
            2, 3, 4
        };
    }
};

// 你可以为其他形状（例如 Sphere）定义特化模板
template<>
struct MeshData<class Sphere> {
    static std::vector<float> GetVertices() {
        // 定义Sphere的顶点数据
    }

    static std::vector<unsigned int> GetIndices() {
        // 定义Sphere的索引数据
    }
};


