#pragma once

#include <vector>

// ͨ��ģ���࣬���ڴ洢�������������
template<typename T>
struct MeshData {
    static std::vector<float> GetVertices();
    static std::vector<unsigned int> GetIndices();
};

// �ػ�ģ������ Cube
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

// �����Ϊ������״������ Sphere�������ػ�ģ��
template<>
struct MeshData<class Sphere> {
    static std::vector<float> GetVertices() {
        // ����Sphere�Ķ�������
    }

    static std::vector<unsigned int> GetIndices() {
        // ����Sphere����������
    }
};


