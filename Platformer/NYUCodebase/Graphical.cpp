//
//  Graphical.cpp
//  NYUCodebase
//
//  Created by Noah Weitz on 4/21/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#include "Graphical.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureHandler::TextureHandler() {}

GLuint TextureHandler::loadTexture(const char* img) {
    int w, h, comp;
    unsigned char* image = stbi_load(img, &w, &h, &comp, STBI_rgb_alpha);
    
    if(image == NULL) {
        std::cout << "Unable to load image\n";
        assert(false);
    }
    
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return retTexture;
}

void TextureHandler::DrawText(ShaderProgram& program, GLuint font, std::string msg, float size, float spacing) {
    float charSize = 1.0/16.0f;
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    for(int i=0; i < msg.size(); i++) {
        int spriteIndex = (int)msg[i];
        float textureX = (float)(spriteIndex % 16) / 16.0f;
        float textureY = (float)(spriteIndex / 16) / 16.0f;
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
        });
        texCoordData.insert(texCoordData.end(), {
            textureX, textureY,
            textureX, textureY + charSize,
            textureX + charSize, textureY,
            textureX + charSize, textureY + charSize,
            textureX + charSize, textureY,
            textureX, textureY + charSize,
        }); }
    
    glUseProgram(program.programID);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, font);
    glDrawArrays(GL_TRIANGLES, 0, msg.size()*6.0f);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

SpriteSheet::SpriteSheet() {}

SpriteSheet::SpriteSheet(float s, unsigned int tid, float xc, float yc, float w, float h) :
size(s), textureID(tid), xCount(xc), yCount(yc), width(w), height(h) {}

void SpriteSheet::Draw(ShaderProgram& program, glm::mat4& modelMatrix, int index, float x, float y) {
    
    float u = 1.0f;
    float v = 1.0f;
    float sprite_width = 1.0f / xCount;
    float sprite_height = 1.0f / yCount;
    
    GLfloat tex_coords[] =
    {
        u, (v + sprite_height),
        (u + sprite_width), v,
        u, v,
        (u + sprite_width), v,
        u, (v + sprite_width),
        (u + sprite_width), (v + sprite_height)
    };
    
    float aspect = width / height;
    float vertices[] =
    {
        (-0.5f * size * aspect), (-0.5f * size),
        (0.5f * size * aspect), (0.5f * size),
        (-0.5f * size * aspect), (0.5f * size),
        (0.5f * size * aspect), (0.5f * size),
        (-0.5f * size * aspect), (-0.5f * size),
        (0.5f * size * aspect), (-0.5f * size)
    };
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, tex_coords);
    
    glEnableVertexAttribArray(program.texCoordAttribute);
    glEnableVertexAttribArray(program.positionAttribute);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, 0.0f));
    program.SetModelMatrix(modelMatrix);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

GLuint SpriteSheet::getTID() { return textureID; }
