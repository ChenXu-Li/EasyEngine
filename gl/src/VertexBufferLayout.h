#pragma once
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#include<vector>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	bool normalized;
	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type){
		case GL_UNSIGNED_BYTE:return 1;
		case GL_FLOAT:return 4;
		case GL_UNSIGNED_INT: return 4;
		}

		return 0;

	}
};
class VertexBufferLayout
{
private:
	unsigned int m_Stride;
	std::vector<VertexBufferElement> m_Elements;

public:
	VertexBufferLayout() :m_Stride(0) {};
	template<typename T>
	void Push(unsigned int count) {
		//std::count<<"must have <type> while using push"<<std::endl£»
		//__debugbreak();
	}
	template<>
	void Push<float>(unsigned int count) {
		m_Elements.push_back({ GL_FLOAT,count,false });
		m_Stride += count*VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}
	template<>
	void Push<unsigned  int>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_INT,count,false });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}
	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE,count,true });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}
	const std::vector<VertexBufferElement> GetElements() const {
		return m_Elements;
	}
	const unsigned int GetStride() const {
		return m_Stride;
	}


};

