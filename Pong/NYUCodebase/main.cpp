#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class Paddle {
    glm::mat4 paddleMatrix;
    float width;
    float height;
    float xPosition;
    float yPosition;
public:
    Paddle(glm::mat4 model, float offsetX, float w, float h):
    paddleMatrix(model), xPosition(offsetX), width(w), height(h) {}
    void moveY(float amt) {yPosition += amt;}
    bool notHittingUpperBoundary() {return yPosition < 0.7;}
    bool notHittingLowerBoundary() {return yPosition > -0.7;}
    float getWidth() { return width;}
    float getHeight() { return height;}
    float getX() {return xPosition;}
    float getY() { return yPosition;}
    
};

class Ball {
    glm::mat4& ballMatrix;
    float widthHeight;
    float xPosition;
    float yPosition;
    bool xDirection;
    bool yDirection;
    bool visible;
public:
    Ball(glm::mat4& model):
    ballMatrix(model), xPosition(0), yPosition(0), widthHeight(0.1) {}
    float getX() { return xPosition;}
    float getY() { return yPosition;}
    void moveY(float amt) {yPosition += amt;}
    void moveX(float amt) {xPosition += amt;}
    bool isVisible() {return visible;}
    bool notHittingUpperBoundary() { return yPosition < 1;}
    bool notHittingLowerBoundary() { return yPosition > -1; }
    bool hitPaddle(float pX, float pY, float pW, float pH) {
        float xDist = std::abs(xPosition - pX) - ((widthHeight + pW)/2);
        float yDist = std::abs(yPosition -pY) - ((widthHeight + pH)/2);
        return xDist < 0 && yDist < 0;
    }
    bool exitedLeft(float pX) {
        return xPosition < pX;
    }
    bool exitedRight(float pX) {
        return xPosition > pX;
    }
    void serve(bool dir) {
        visible = true;
        yDirection = true;
        if(dir) {
            xDirection = true;
        }
        else{
            xDirection = false;
        }
    }
    void move(Paddle& left, Paddle& right) {
        if(yDirection) {
            moveY(0.001f);
        }
        else {
            moveY(-0.001f);
        }
        if(xDirection) {
            moveX(0.001f);
        }
        else {
            moveX(-0.001f);
        }
        if(!notHittingUpperBoundary() || !notHittingLowerBoundary()) { yDirection = !yDirection;}
        if(hitPaddle(left.getX(), left.getY(), left.getWidth(), left.getHeight())) { xDirection = true;}
        if(hitPaddle(right.getX(), right.getY(), right.getWidth(), right.getHeight())) { xDirection = false;}
        ballMatrix = glm::mat4(1.0f);
        ballMatrix = glm::translate(ballMatrix, glm::vec3(xPosition, yPosition, 0.0f));
        ballMatrix = glm::scale(ballMatrix, glm::vec3(-0.10f, -0.10f, 1.0f));
    }
    void reset() {
        visible = false;
        xPosition = 0;
        yPosition = 0;
        ballMatrix = glm::mat4(1.0f);
        ballMatrix = glm::translate(ballMatrix, glm::vec3(xPosition, yPosition, 0.0f));
        ballMatrix = glm::scale(ballMatrix, glm::vec3(-0.10f, -0.10f, 1.0f));
        visible = true;
    }
};

SDL_Window* displayWindow;

void draw(ShaderProgram& program, float vertices[], glm::mat4& modelMatrix) {
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    program.SetModelMatrix(modelMatrix);
    program.SetColor(2555.0f, 255.0f, 155.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
}

void checkForScore(Paddle& l, Paddle& r, Ball& b) {
    if(b.exitedLeft(l.getX())) {
        b.reset();
    }
    if(b.exitedRight(r.getX())) {
        b.reset();
    }
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif
    
    float lastFrameTicks = 0.0f;

    bool lastScore = false; //if false, right was the last score; if true, left was the last score
    bool served = false;
    
    float leftVertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    float rightVertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5f};
    float ballVertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5f};
    
    glViewport(0, 0, 800, 600);
    
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    
    glm::mat4 ballModelMatrix = glm::mat4(1.0f);
    glm::mat4 leftModelMatrix = glm::mat4(1.0f);
    glm::mat4 rightModelMatrix = glm::mat4(1.0f);
    
    Paddle leftPaddle(leftModelMatrix, -1.65, 0.15, 0.65);
    Paddle rightPaddle(rightModelMatrix, 1.65, 0.15, 0.65);
    Ball ball(ballModelMatrix);
    
    projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    leftModelMatrix = glm::translate(leftModelMatrix, glm::vec3(-1.65f, 0.0f, 0.0f));
    leftModelMatrix = glm::scale(leftModelMatrix, glm::vec3(-0.15f, -0.65f, 1.0f));
    
    rightModelMatrix = glm::translate(rightModelMatrix, glm::vec3(1.65f, 0.0f, 0.0f));
    rightModelMatrix = glm::scale(rightModelMatrix, glm::vec3(-0.15f, -0.65f, 1.0f));
    
    ballModelMatrix = glm::translate(ballModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    ballModelMatrix = glm::scale(ballModelMatrix, glm::vec3(-0.10f, -0.10f, 1.0f));
    
    glUseProgram(program.programID);

    SDL_Event event;
    bool done = false;
    while (!done) {
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_a) { //Move the left paddle up
                    if(leftPaddle.notHittingUpperBoundary()) {
                        leftPaddle.moveY(0.1f);
                        leftModelMatrix = glm::mat4(1.0f);
                        leftModelMatrix = glm::translate(leftModelMatrix, glm::vec3(leftPaddle.getX(), leftPaddle.getY(), 0.0f));
                        leftModelMatrix = glm::scale(leftModelMatrix, glm::vec3(-0.15f, -0.65f, 1.0f));
                    }
                }
                else if(event.key.keysym.sym == SDLK_z) {
                    if(leftPaddle.notHittingLowerBoundary()) { //Move the left paddle down
                        leftPaddle.moveY(-0.1f);
                        leftModelMatrix = glm::mat4(1.0f);
                        leftModelMatrix = glm::translate(leftModelMatrix, glm::vec3(leftPaddle.getX(), leftPaddle.getY(), 0.0f));
                        leftModelMatrix = glm::scale(leftModelMatrix, glm::vec3(-0.15f, -0.65f, 1.0f));
                    }
                    
                }
            }
        }
        
        if(rightPaddle.getY() < ball.getY()) {
            if(rightPaddle.notHittingUpperBoundary()) {
                rightPaddle.moveY(0.0005f);
                rightModelMatrix = glm::mat4(1.0f);
                rightModelMatrix = glm::translate(rightModelMatrix, glm::vec3(rightPaddle.getX(), rightPaddle.getY(), 0.0f));
                rightModelMatrix = glm::scale(rightModelMatrix, glm::vec3(-0.15f, -0.65f, 1.0f));
            }
        }
        else if(rightPaddle.getY() > ball.getY()) {
            if(rightPaddle.notHittingLowerBoundary()) {
                rightPaddle.moveY(-0.0005f);
                rightModelMatrix = glm::mat4(0.1f);
                rightModelMatrix = glm::translate(rightModelMatrix, glm::vec3(rightPaddle.getX(), rightPaddle.getY(), 0.0f));
                rightModelMatrix = glm::scale(rightModelMatrix, glm::vec3(-0.15f, -0.65f, 1.0f));
            }
        }
        
        if(!served) {
            ball.serve(lastScore);
            served = true;
        }
        ball.move(leftPaddle, rightPaddle);
        checkForScore(leftPaddle, rightPaddle, ball);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        program.SetViewMatrix(viewMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        
        draw(program, leftVertices, leftModelMatrix);
        draw(program, rightVertices, rightModelMatrix);
        if(ball.isVisible()) {draw(program, ballVertices, ballModelMatrix);}
        
        SDL_GL_SwapWindow(displayWindow);
        
    }
    
    SDL_Quit();
    return 0;
}
