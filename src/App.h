//
// Created by lasagnaphil on 2/6/18.
//

#ifndef GENGINE_APP_H
#define GENGINE_APP_H

#define GLM_FORCE_RADIANS 1

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SDL.h"
#include <yaml-cpp/yaml.h>

#include <memory>

#include "glad/glad.h"
#include "Shader.h"
#include "Texture.h"
#include "Scene.h"
#include "nodes/MeshNode.h"
#include "SceneData.h"

class App {
public:
    ~App();
    void start();
    void loadScene();
    void processInput();
    void update(float dt);
    void render();

    static constexpr Uint32 msPerFrame = 16;

private:
    SDL_Window* window;
    SDL_GLContext mainContext;
    bool quit = false;

    Scene* scene;

    SceneData sceneData;
};


#endif //GENGINE_GAME_H
