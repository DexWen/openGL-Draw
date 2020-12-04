/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "spriteRenderer.h"


SpriteRenderer::SpriteRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec4 color, glm::vec2 anchor, glm::vec2 scale)
{
    // Prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec2 newSize = glm::vec2(size.x * scale.x, size.y * scale.y);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-anchor.x * newSize.x, -anchor.y * newSize.y, 0.0f));

    model = glm::scale(model, glm::vec3(newSize.x, newSize.y, 1.0f)); 
    
    this->shader.SetMatrix4("model", model);
    this->shader.SetVector4f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);


    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// glFlush();
    // glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = { 
        0.0f,1.0f,0.0f,1.0f,  // letf top 
        1.0f,0.0f,1.0f,0.0f,  // right down
        0.0f,0.0f,0.0f,0.0f,  // left down
        0.0f,1.0f,0.0f,1.0f,  // 
        1.0f,1.0f,1.0f,1.0f,
        1.0f,0.0f,1.0f,0.0f
    };

    // 测试纹理环绕模式的VBO
    /*
    GLfloat vertices[] = { 
        0.0f,1.0f,0.0f,1.2f,  // letf top 
        1.0f,0.0f,1.2f,0.0f,  // right down
        0.0f,0.0f,0.0f,0.0f,  // left down
        0.0f,1.0f,0.0f,1.2f,  // 
        1.0f,1.0f,1.2f,1.2f,
        1.0f,0.0f,1.2f,0.0f
    };*/

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);

    // GLfloat vertices[] = {
    //     1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    //     1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    //     -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    //     -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    // };

    // GLuint indices[] = {  // Note that we start from 0!
    //     0, 1, 3, // First Triangle
    //     1, 2, 3  // Second Triangle
    // };
    // GLuint VBO, EBO;
    // glGenVertexArrays(1, &this->quadVAO);
    // glGenBuffers(1, &VBO);
	// glGenBuffers(1, &EBO);
	// glBindVertexArray(this->quadVAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    // glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    // glEnableVertexAttribArray(1);
    
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    // glEnableVertexAttribArray(2);

    // glBindVertexArray(0);
}