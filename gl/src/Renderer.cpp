#include "Renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OPenGl Error](" << error << ")" << function << " " <<
            file << " " << line << std::endl;
        return GL_FALSE;
    }
    return GL_TRUE;
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ebo, const Shader& shader) const
{
    vao.Bind();
    ebo.Bind();
    shader.Bind();
    glLineWidth(2.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GLCall(glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, 0));
   //GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}
