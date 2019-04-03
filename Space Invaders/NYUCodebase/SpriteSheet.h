//
//  SpriteSheet.h
//  NYUCodebase
//
//  Created by Noah Weitz on 4/2/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#ifndef SpriteSheet_h
#define SpriteSheet_h

#include "ShaderProgram.h"

class SpriteSheet {
private:
    unsigned int textureID;
    float u;
    float v;
    float w;
    float h;
public:
    SpriteSheet();
    SpriteSheet(unsigned int texID, float uu, float vv, float ww, float hh);
    
    void Draw(ShaderProgram& program);
    float getU();
    float getV();
    float getW();
    float getH();
    unsigned int getTID();
    
};

#endif /* SpriteSheet_h */
