/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef PAINTER_H
#define PAINTER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// Painter holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Painter
{
public:
    GLboolean              Keys[1024];
    GLuint                 Width, Height;
    // Constructor/Destructor
    Painter(GLuint width, GLuint height);
    ~Painter();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
};

#endif