//
//  Graphical.h
//  NYUCodebase
//
//  Created by Noah Weitz on 4/21/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#ifndef Graphical_h
#define Graphical_h

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
//
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "ShaderProgram.h"

#include <vector>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class TextureHandler {
public:
    TextureHandler();
    static GLuint loadTexture(const char* img);
    static void DrawText(ShaderProgram& program, GLuint font, std::string msg, float size, float spacing);
};

class SpriteSheet {
private:
    float size;
    unsigned int textureID;
    float xCount;
    float yCount;
    float width;
    float height;
public:
    SpriteSheet();
    SpriteSheet(float s, unsigned int tid, float xc, float yc, float w, float h);
    void Draw(ShaderProgram& program, glm::mat4& modelMatrix, int index, float x, float y);
    GLuint getTID();
};

#endif /* Graphical_h */
