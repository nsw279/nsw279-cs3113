//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Noah Weitz on 4/2/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#include <stdio.h>

#include "Entity.h"

Entity::Entity(bool allegiance, GLuint graphic) : type(allegiance), graphic(graphic) {
    height = 1.0f;
}

void Entity::draw(ShaderProgram& program, glm::mat4& modelMatrix) {
    if (alive) {
        float vertices[] = { -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
        
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float texCoords[] = {0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};
        
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, yPos, 0.0f));
        program.SetModelMatrix(modelMatrix);
        
        glBindTexture(GL_TEXTURE_2D, graphic);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
    }
}

float Entity::getX() { return xPos; }
float Entity::getY() { return yPos; }
float Entity::getWidth() { return width; }
float Entity::getHeight() { return height; }

void Entity::shootBullet(bool player, Bullet b) {
    b.beShot(player, xPos, yPos+0.5f, width, height);
}

bool Entity::colliding(float otherX, float otherY, float otherW, float otherH) {
    float px = abs(xPos - otherX) - ((width+otherW) / 2);
    float py = abs(yPos - otherY) - ((height+otherH) / 2);
    return px < 0 && py < 0 && alive;
}

void Entity::moveX(float d) { xPos += d; }

void Entity::moveY(float d) { xPos += d; }

void Entity::destroy() {
    if (type) {
        alive = false;
        xPos = 0;
        alive = true;
    }
    else {
        alive = false;
    }
}
