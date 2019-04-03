////
////  Loaders.cpp
////  NYUCodebase
////
////  Created by Noah Weitz on 4/2/19.
////  Copyright © 2019 Ivan Safrin. All rights reserved.
////
//
//#include <stdio.h>
//#include "Loaders.h"
//
//GLuint Loaders::LoadTexture(const char *filePath) {
//    int w, h, comp;
//    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
//
//    if(image == NULL) {
//        std::cout << "Unable to load image\n";
//        assert(false);
//    }
//
//    GLuint retTexture;
//    glGenTextures(1, &retTexture);
//    glBindTexture(GL_TEXTURE_2D, retTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    stbi_image_free(image);
//    return retTexture;
//}
//
//void Loaders::DrawText(ShaderProgram& program, GLuint font, std::string text, float size, float space) {
//    float texture_size = 1.0 / 16.0f;
//    std::vector<float> vertexData;
//    std::vector<float> texCoordData;
//    for (int i = 0; i < text.size(); i++) {
//        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
//        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
//        vertexData.insert(vertexData.end(), {
//            ((size + space) * i) + (-0.5f * size), 0.5f * size,
//            ((size + space) * i) + (-0.5f * size), -0.5f * size,
//            ((size + space) * i) + (0.5f * size), 0.5f * size,
//            ((size + space) * i) + (0.5f * size), -0.5f * size,
//            ((size + space) * i) + (0.5f * size), 0.5f * size,
//            ((size + space) * i) + (-0.5f * size), -0.5f * size,
//        });
//        texCoordData.insert(texCoordData.end(), {
//            texture_x, texture_y,
//            texture_x, texture_y + texture_size,
//            texture_x + texture_size, texture_y,
//            texture_x + texture_size, texture_y + texture_size,
//            texture_x + texture_size, texture_y,
//            texture_x, texture_y + texture_size,
//        });
//    }
//    glUseProgram(program.programID);
//    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
//    glEnableVertexAttribArray(program.positionAttribute);
//    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
//    glEnableVertexAttribArray(program.texCoordAttribute);
//    glBindTexture(GL_TEXTURE_2D, font);
//    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6.0f);
//    glDisableVertexAttribArray(program.positionAttribute);
//    glDisableVertexAttribArray(program.texCoordAttribute);
//}
//
//void Loaders::DrawSheet(ShaderProgram& program, glm::mat4 modelMatrix, int index, int sprX, int sprY, int textureID, float xPos, float yPos) {
//    float u = (float)(((int)index) % sprX) / (float)sprX;
//    float v = (float)(((int)index) / sprX) / (float)sprY;
//    float spriteWidth = 1.0 / (float)sprX;
//    float spriteHeight = 1.0 / (float)sprY;
//
//    GLfloat texCoords[] = {
//        u, v + spriteHeight,
//        u + spriteWidth, v,
//        u, v,
//        u + spriteWidth, v,
//        u, v + spriteHeight,
//        u + spriteWidth, v + spriteHeight,
//    };
//
//    float vertices[] = { -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
//
//    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
//    glEnableVertexAttribArray(program.positionAttribute);
//
//    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
//    glEnableVertexAttribArray(program.texCoordAttribute);
//
//    modelMatrix = glm::mat4(1.0f);
//    modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, yPos, 0.0f));
//    program.SetModelMatrix(modelMatrix);
//
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//
//    glDisableVertexAttribArray(program.positionAttribute);
//    glDisableVertexAttribArray(program.texCoordAttribute);
//}
