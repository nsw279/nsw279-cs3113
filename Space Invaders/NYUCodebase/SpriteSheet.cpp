//
//  SpriteSheet.cpp
//  NYUCodebase
//
//  Created by Noah Weitz on 4/2/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#include <stdio.h>
#include "SpriteSheet.h"

SpriteSheet::SpriteSheet() {}

SpriteSheet::SpriteSheet(unsigned int texID, float uu, float vv, float ww, float hh) :
textureID(texID), u(uu), v(vv), w(ww), h(hh) {}

float SpriteSheet::getU() { return u; }
float SpriteSheet::getV() { return v; }
float SpriteSheet::getH() { return h; }
float SpriteSheet::getW() { return w; }
unsigned int SpriteSheet::getTID() { return textureID; }
