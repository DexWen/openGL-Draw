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
SpriteRenderer  *Renderer;


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
    ResourceManager::LoadShader("E:/myGit/openGL-Draw/src/shader/shader.vs", "E:/myGit/openGL-Draw/src/shader/shader.frag", nullptr, "sprite");
    // Configure shaders
    // glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    // ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    // ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // Load textures
    ResourceManager::LoadTexture("E:/myGit/openGL-Draw/src/material/gtaBg2.jpg", GL_TRUE, "bg");
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
    Renderer->DrawSprite(bgTexture, glm::vec2(0, 0), glm::vec2(bgTexture.Width, bgTexture.Height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}