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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *filePath) {
    int w, h, comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    
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

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("2D Scene", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    float lastFrameTicks = 0.0f;
    float angle = 45.0f * (3.1415926f / 180.0f);
    
    glViewport(0, 0, 640, 360);
    
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    ShaderProgram textureProg;
    textureProg.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    // I wasn't entirely sure how to get the correct filepath outside of my own machine, these paths may need to be changed
    GLuint medal1 = LoadTexture("/nsw279-cs3113/HW2/NYUCodebase/medals/flat_medal1.png");
    GLuint smile = LoadTexture("/nsw279-cs3113/HW2/NYUCodebase/smilies/smile.gif");
    GLuint cool = LoadTexture("/nsw279-cs3113/HW2/NYUCodebase/smilies/cool.gif");
    
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    
    glm::mat4 modelMatrix2 = glm::mat4(1.0f);
    glm::mat4 modelMatrix3 = glm::mat4(1.0f);
    
    projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(-1.0f, 0.4f, 0.0f));
    modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(-0.5f, 0.5f, 0.0f));
    
    modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(1.0f, -0.6f, 0.0f));
    modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(-0.5f, 0.5f, 0.0f));
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        
        //modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 0.1f));
        
        glClearColor(0.0f, 0.0f, 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(program.programID);
        
        program.SetViewMatrix(viewMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        program.SetModelMatrix(modelMatrix);
        
        float vertices2[] = {-0.1, -0.1, 0.9, -0.1, 0.9, 0.9, -0.1, -0.1, 0.9, 0.9, -0.1, 0.9};
        
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
        glEnableVertexAttribArray(program.positionAttribute);
    
        program.SetColor(255.0f, 255.0f, 255.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glDisableVertexAttribArray(program.positionAttribute);

        float vertices[] = {0.2f, -0.8f, -0.3f, 0.2f, -0.8f, -0.8f};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program.positionAttribute);

        program.SetColor(0.0f, 255.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        program.SetViewMatrix(viewMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        program.SetModelMatrix(modelMatrix);

        glDisableVertexAttribArray(program.positionAttribute);
        
        glUseProgram(textureProg.programID);
        
        textureProg.SetViewMatrix(viewMatrix);
        textureProg.SetProjectionMatrix(projectionMatrix);
        textureProg.SetModelMatrix(modelMatrix);
        
        glBindTexture(GL_TEXTURE_2D, medal1);
        
        float vertices3[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        
        glVertexAttribPointer(textureProg.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
        glEnableVertexAttribArray(textureProg.positionAttribute);
        
        float textureCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        
        glVertexAttribPointer(textureProg.texCoordAttribute, 2, GL_FLOAT, false, 0, textureCoords);
        glEnableVertexAttribArray(textureProg.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        textureProg.SetViewMatrix(viewMatrix);
        textureProg.SetProjectionMatrix(projectionMatrix);
        textureProg.SetModelMatrix(modelMatrix2);
        
        glBindTexture(GL_TEXTURE_2D, smile);
        
        float vertices4[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        
        glVertexAttribPointer(textureProg.positionAttribute, 2, GL_FLOAT, false, 0, vertices4);
        glEnableVertexAttribArray(textureProg.positionAttribute);
        
        float textureCoords2[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        
        glVertexAttribPointer(textureProg.texCoordAttribute, 2, GL_FLOAT, false, 0, textureCoords2);
        glEnableVertexAttribArray(textureProg.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        textureProg.SetViewMatrix(viewMatrix);
        textureProg.SetProjectionMatrix(projectionMatrix);
        textureProg.SetModelMatrix(modelMatrix3);
        
        glBindTexture(GL_TEXTURE_2D, cool);
        
        float vertices5[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        
        glVertexAttribPointer(textureProg.positionAttribute, 2, GL_FLOAT, false, 0, vertices5);
        glEnableVertexAttribArray(textureProg.positionAttribute);
        
        float textureCoords3[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        
        glVertexAttribPointer(textureProg.texCoordAttribute, 2, GL_FLOAT, false, 0, textureCoords3);
        glEnableVertexAttribArray(textureProg.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        SDL_GL_SwapWindow(displayWindow);
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        angle += elapsed / 5000000;
    }
    
    SDL_Quit();
    return 0;
}
