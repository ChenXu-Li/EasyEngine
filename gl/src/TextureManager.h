#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Texture.h"

class TextureManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_TextureCache;
    // ˽�й��캯������������
    TextureManager() = default;
    ~TextureManager() = default;

public:
    // ���ÿ�������͸�ֵ����
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    // ��ȡ����ʵ��
    static TextureManager& GetInstance() {
        static TextureManager instance;
        return instance;
    }


    std::shared_ptr<Texture> GetTexture(const std::string& filepath) {
        // Check if the shader already exists in the cache
        auto it = m_TextureCache.find(filepath);
        if (it != m_TextureCache.end()) {
            // Return the existing shader
            return it->second;
        }

        // Create a new shader and add it to the cache
        std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(filepath);
        m_TextureCache[filepath] = newTexture;

        return newTexture;
    }

    void Clear() {
        m_TextureCache.clear();
    }
};
