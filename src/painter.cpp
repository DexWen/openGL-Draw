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


Painter::Painter(GLuint width, GLuint height) 
	: Keys(), Width(width), Height(height) 
{ 

}

Painter::~Painter()
{
    delete Renderer;
}

void Painter::Init()
{
    // Load shaders
    ResourceManager::LoadShader("shader/sprite.vs", "shader/sprite.frag", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // Load textures
    ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
    // Set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
}

void Painter::Update(GLfloat dt)
{

}


void Painter::ProcessInput(GLfloat dt)
{

}

void Painter::Render()
{
    Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}