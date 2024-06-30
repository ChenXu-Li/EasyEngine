#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Shader.h"

class ShaderManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_ShaderCache;
    // ˽�й��캯������������
    ShaderManager() = default;
    ~ShaderManager() = default;

public:
    // ���ÿ�������͸�ֵ����
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

    // ��ȡ����ʵ��
    static ShaderManager& GetInstance() {
        static ShaderManager instance;
        return instance;
    }


    std::shared_ptr<Shader> GetShader(const std::string& filepath) {
        // Check if the shader already exists in the cache
        auto it = m_ShaderCache.find(filepath);
        if (it != m_ShaderCache.end()) {
            // Return the existing shader
            return it->second;
        }

        // Create a new shader and add it to the cache
        std::shared_ptr<Shader> newShader = std::make_shared<Shader>(filepath);
        m_ShaderCache[filepath] = newShader;

        return newShader;
    }

    void Clear() {
        m_ShaderCache.clear();
    }
};
