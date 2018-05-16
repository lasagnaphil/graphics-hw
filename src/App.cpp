//
// Created by lasagnaphil on 2/6/18.
//

#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>
#include "App.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Mesh.h"
#include "nodes/MeshNode.h"
#include "AppSettings.h"
#include "InputManager.h"
#include "nodes/FirstPersonCamera.h"
#include "nodes/LightNode.h"
#include "nodes/TrackballCamera.h"
#include "SweptSurface.h"

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

    // Use OpenGL Version 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup ImGui binding
    ImGui::CreateContext();
    ImGui_ImplSdlGL3_Init(window);
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    // Load textures
    stbi_set_flip_vertically_on_load(true);

    loadScene();

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

void App::loadScene() {
    // Shaders
    sceneData.loadResources("resources/scene.yml");

    // Load the swept surface
    std::shared_ptr<Mesh> mesh = SweptSurface::constructFromFile("resources/screwbar.txt", sceneData.getMaterial("default"));
    sceneData.addMesh("swept_surface", mesh);

    scene = sceneData.loadSceneGraph("resources/scene.yml");

    scene->constructBSPTree();
}

void App::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSdlGL3_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            quit = true;
            break;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_1: {
                    MeshNode* sweptSurface = scene->getRootNode()->query("Swept Surface")->cast<MeshNode>();
                    sweptSurface->setShader(sceneData.getShader("default"));
                    break;
                }
                case SDLK_2: {
                    MeshNode* sweptSurface = scene->getRootNode()->query("Swept Surface")->cast<MeshNode>();
                    sweptSurface->setShader(sceneData.getShader("wireframe"));
                    break;
                }
                case SDLK_r: {
                    sceneData.clear();
                    loadScene();
                }
                case SDLK_SPACE: {
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
}

void App::render() {
    ImGui_ImplSdlGL3_NewFrame(window);

    // clear screen
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // do stuff
    scene->render();

    const char* items[] = {"Screwbar", "Screwbar (Catmull-Rom)", "Trombone", "Coke Bottle"};
    static const char* selected = items[0];
    static ImGuiComboFlags flags = 0;

    ImGui::Begin("Application Settings");
    if (ImGui::BeginCombo("Mesh", selected, flags)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (selected == items[n]);
            if (ImGui::Selectable(items[n], is_selected)) {
                selected = items[n];
                std::shared_ptr<Mesh> mesh;
                switch(n) {
                    case 0:
                        mesh = SweptSurface::constructFromFile("resources/screwbar.txt", sceneData.getMaterial("default"));
                        break;
                    case 1:
                        mesh = SweptSurface::constructFromFile("resources/screwbar_cr.txt", sceneData.getMaterial("default"));
                        break;
                    case 2:
                        mesh = SweptSurface::constructFromFile("resources/trombone.txt", sceneData.getMaterial("default"));
                        break;
                    case 3:
                        mesh = SweptSurface::constructFromFile("resources/coke_bottle.txt", sceneData.getMaterial("default"));
                        break;
                }
                sceneData.addMesh("swept_surface", mesh);
                MeshNode* sweptSurface = scene->getRootNode()->query("Swept Surface")->cast<MeshNode>();
                sweptSurface->setMesh(mesh);
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
        }
        ImGui::EndCombo();
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
}

App::~App() {
    SDL_DestroyWindow(window);
}
