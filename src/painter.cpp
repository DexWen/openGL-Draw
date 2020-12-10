/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "painter.h"
#include "resourceManager.h"
#include "spriteRenderer.h"


// Painter-related State data
SpriteRenderer *Renderer;


Painter::Painter(GLuint width, GLuint height): Keys(), Width(width), Height(height)
{ 

}

Painter::~Painter()
{
    delete Renderer;
}

void Painter::Init()
{
    // Load shaders
    ResourceManager::LoadShader("E:/myGit/openGL-Draw/src/shader/sprite.vs", "E:/myGit/openGL-Draw/src/shader/sprite.frag", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), 0.0f, static_cast<GLfloat>(this->Height), -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // Load textures
	ResourceManager::LoadTexture("E:/myGit/openGL-Draw/src/material/gtaBg2.jpg", GL_TRUE, "bg");
	ResourceManager::LoadTexture("E:/myGit/openGL-Draw/src/material/sun.jpg", GL_TRUE, "logo");
	// Set render-specific controls
	Shader spShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(spShader);
}

void Painter::Update(GLfloat dt)
{

}


void Painter::ProcessInput(GLfloat dt)
{

}

void Painter::Render()
{	
	Texture2D bgTexture = ResourceManager::GetTexture("bg");
    // position size rotation color
    // texture, position, size, rotation, color, anchor, scale
    glm::vec2 position = glm::vec2(this->Width/2, this->Height/2);
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec2 size = glm::vec2(this->Width, this->Height);
	GLfloat rotateAngle = 0.0f;
	GLfloat rotate = 3.141592f * rotateAngle / 180.0f;
    glm::vec2 anchor = glm::vec2(0.5f, 0.5f);
	glm::vec2 scale = glm::vec2(1.0f, 1.0f);
    Renderer->DrawSprite(bgTexture, position, size, rotate, color, anchor, scale);

	Texture2D logoTexture = ResourceManager::GetTexture("logo");
    // position size rotation colors
    // texture, position, size, rotation, color, anchor, scale
    glm::vec2 position2 = glm::vec2(this->Width/2, this->Height/2);
    glm::vec4 color2 = glm::vec4(162.0f/255.0f, 205.0f/255.0f, 90.0f/255.0f, 1.0f);
	glm::vec2 size2 = glm::vec2(logoTexture.Width, logoTexture.Height);
	GLfloat rotateAngle2 = 0.0f;
	GLfloat rotate2 = 3.141592f * rotateAngle / 180.0f;
    glm::vec2 anchor2 = glm::vec2(0.5f, 0.5f);
	glm::vec2 scale2 = glm::vec2(1.0f, 1.0f);
    Renderer->DrawSprite(logoTexture, position2, size2, rotate2, color2, anchor2, scale2);
}