#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<TextureInfo> textureinfos)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textureinfos = textureinfos;

    this->setupMesh();
}

void Mesh::Draw(Shader& shader)
{
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for (GLuint i = 0; i < this->textureinfos.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // �ڰ�����ǰ��Ҫ�����ʵ�������Ԫ
        // �����������к� (N in diffuse_textureN)
        std::stringstream ss;
        std::string number;
        std::string name = this->textureinfos[i].type;
        if (name == "texture_diffuse")
            ss << diffuseNr++; // ��GLuin���뵽string stream
        else if (name == "texture_specular")
            ss << specularNr++; // ��GLuin���뵽string stream
        number = ss.str();

        shader.SetUniform1i("material." + name + number, i);

        shader.SetUniform1i("u_Texture", i);
       // glUniform1f(glGetUniformLocation(shader.Program, ("material." + name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, this->textureinfos[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    shader.Bind();
    // ����Mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::FindBestValue(float& max_x, float& min_x, float& max_y, float& min_y, float& max_z, float& min_z)
{
    // ��ʼ�������Сֵ
    max_x = min_x = this->vertices[0].Position.x;
    max_y = min_y = this->vertices[0].Position.y;
    max_z = min_z = this->vertices[0].Position.z;

    for (const auto& vertex : this->vertices)
    {
        // ���������Сֵ
        if (vertex.Position.x > max_x) max_x = vertex.Position.x;
        if (vertex.Position.x < min_x) min_x = vertex.Position.x;

        if (vertex.Position.y > max_y) max_y = vertex.Position.y;
        if (vertex.Position.y < min_y) min_y = vertex.Position.y;

        if (vertex.Position.z > max_z) max_z = vertex.Position.z;
        if (vertex.Position.z < min_z) min_z = vertex.Position.z;
    }
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
        &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
        &this->indices[0], GL_STATIC_DRAW);

    // ���ö�������ָ��
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid*)0);
   
    // ���ö������������
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid*)offsetof(Vertex, TexCoords));

    // ���÷���ָ��
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}
