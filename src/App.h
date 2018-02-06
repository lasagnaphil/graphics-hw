//
// Created by lasagnaphil on 2/6/18.
//

#ifndef GENGINE_APP_H
#define GENGINE_APP_H

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>
#include <memory>

#include "glad/glad.h"
#include "Shader.h"
#include "Texture.h"

class App {
public:
    ~App();
    void start();
    void processInput();
    void update(float dt);
    void render();

    static constexpr Uint32 msPerFrame = 16;

private:
    SDL_Window* window;
    SDL_GLContext mainContext;
    bool quit = false;

    std::unique_ptr<Shader> triangleShader;
    unsigned int vbo, vao, ebo;
    std::unique_ptr<Texture> containerTex, faceTex;

    float vertices[32] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    unsigned int indices[6] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };
};


#endif //GENGINE_GAME_H
