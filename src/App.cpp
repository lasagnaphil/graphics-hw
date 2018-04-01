//
// Created by lasagnaphil on 2/6/18.
//

#include "App.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Mesh.h"
#include "nodes/MeshNode.h"
#include "AppSettings.h"
#include "InputManager.h"
#include "nodes/Camera.h"
#include "Model.h"
#include "nodes/LightNode.h"
#include "nodes/ModelNode.h"

static void sdl_die(const char * message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    exit(2);
}

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar *message,
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

void App::start() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdl_die("Couldn't initialize SDL");
    atexit(SDL_Quit);
    SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.

    // Use OpenGL Version 4.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    // Create the window
    window = SDL_CreateWindow(
            "GEngine [OpenGL]",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            AppSettings::ScreenWidth, AppSettings::ScreenHeight, SDL_WINDOW_OPENGL
    );

    if (window == NULL) sdl_die("Couldn't set video mode");

    // SDL Settings
    // SDL_SetRelativeMouseMode(SDL_TRUE);

    // Create OpenGL Context
    mainContext = SDL_GL_CreateContext(window);
    if (mainContext == NULL)
        sdl_die("Failed to create OpenGL context");

    // Check OpenGL properties
    printf("OpenGL loaded\n");
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    // Enable the debug callback
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(
                GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true
        );
    }

    // Use v-sync
    SDL_GL_SetSwapInterval(1);

    // Set OpenGL viewport
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Load textures
    stbi_set_flip_vertically_on_load(true);

    //
    // Preparation
    //

    // Shaders
    Shader defaultShader("shaders/lighting.vert", "shaders/lighting.frag");
    defaultShader.use();

    sceneData.setDefaultShader(defaultShader);
    this->scene = sceneData.load("resources/scene.yml");

    // Program loop
    Uint32 frameTime;
    Uint32 lastFrameTime = SDL_GetTicks();

    while (!quit) {
        frameTime = SDL_GetTicks();

        processInput();

        Uint32 dt = frameTime - lastFrameTime;
        lastFrameTime = frameTime;
        update(static_cast<float>(dt) / 1000.f);

        render();

        SDL_GL_SwapWindow(window);

        Uint32 totalFrameTime = SDL_GetTicks() - frameTime;
        if (totalFrameTime < msPerFrame) {
            SDL_Delay(msPerFrame - (SDL_GetTicks() - frameTime));
        }
    }
}

void App::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
            break;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_r: {
                    scene = sceneData.load("resources/scene.yml");
                    auto camera = sceneData.getCamera();
                    if (camera->mouseMovementEnabled) {
                        SDL_SetRelativeMouseMode(SDL_TRUE);
                    } else {
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                    }
                    break;
                }
                case SDLK_SPACE: {
                    auto camera = sceneData.getCamera();
                    camera->mouseMovementEnabled = !camera->mouseMovementEnabled;
                    if (camera->mouseMovementEnabled) {
                        SDL_SetRelativeMouseMode(SDL_TRUE);
                    } else {
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                    }
                    break;
                }
                case SDLK_ESCAPE:
                    quit = true;
                    break;
            }
        }
        InputManager::getInstance()->handleEvents(event);
        scene->processInput(event);
    }
}

void App::update(float dt) {
    static float time = 0.f;
    time += dt;

    scene->update(dt);

    Node* root = scene->getRootNode();
    float tiltAngle = glm::radians(12.5f * std::sin(time));

    auto string1 = root->query("ceiling.string1")->cast<Spatial>();
    string1->setRotationEuler({tiltAngle, 0.f, 0.f});
    auto string1_end = root->query("ceiling.string1_end")->cast<Spatial>();
    string1_end->setRotationEuler({tiltAngle, glm::radians(180.f), 0.f});
    //string1_end->rotate(0.2f * time, {0.f, 1.f, 0.f});

    auto pole1 = string1_end->query("pole1")->cast<Spatial>();
    pole1->setRotationEuler({glm::radians(90.f) + 0.4 * tiltAngle, 0.f, 0.f});

    auto pole1_left_end = string1_end->query("pole1_left_end")->cast<Spatial>();
    auto pole1_right_end = string1_end->query("pole1_right_end")->cast<Spatial>();
    auto pole1_middle1_end = string1_end->query("pole1_middle1_end")->cast<Spatial>();
    auto pole1_middle2_end = string1_end->query("pole1_middle2_end")->cast<Spatial>();
    std::vector<Spatial*> pole1_ends = {pole1_left_end, pole1_right_end, pole1_middle1_end, pole1_middle2_end};
    for (auto pole1_end : pole1_ends) {
        pole1_end->setRotationEuler({0.4f * tiltAngle, 0.f, 0.f});
    }

    auto string2 = pole1_left_end->query("string2")->cast<Spatial>();
    auto string3 = pole1_right_end->query("string3")->cast<Spatial>();
    auto string4 = pole1_middle1_end->query("string4")->cast<Spatial>();
    auto string4_end = pole1_middle1_end->query("string4_end")->cast<Spatial>();
    auto string5 = pole1_middle2_end->query("string5")->cast<Spatial>();
    auto string5_end = pole1_middle2_end->query("string5_end")->cast<Spatial>();
    std::vector<Spatial*> strings = {string2, string3, string4, string4_end, string5, string5_end};
    for (auto string : strings) {
        string->setRotationEuler({0.4f * tiltAngle, 0.f, 0.f});
    }

    auto deco1 = string2->query("deco1")->cast<Spatial>();
    deco1->setRotationEuler({0.5f * (time + 1.f), 0.5f * (time + 2.f), 0.5f * time});
    auto deco2 = string3->query("deco2")->cast<Spatial>();
    deco2->setRotationEuler({0.5f * time, 0.5f * (-time + 2.f), 0.5f * (time + 1.f)});
    auto deco3 = string4_end->query("deco3")->cast<Spatial>();
    deco3->setRotationEuler({0.5f * (-time + 2.f), 0.5f * time, 0.5f * (time + 1.f)});
    auto deco4 = string5_end->query("deco4")->cast<Spatial>();
    deco4->setRotationEuler({0.5f * (time + 1.f), 0.5f * (time + 2.f), 0.5f * (-time)});
}

void App::render() {
    // clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // do stuff
    scene->render();
}

App::~App() {
    SDL_DestroyWindow(window);
}

