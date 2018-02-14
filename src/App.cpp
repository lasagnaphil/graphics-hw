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
    atexit (SDL_Quit);
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
    SDL_SetRelativeMouseMode(SDL_TRUE);

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
    int w,h;
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
    // Shader defaultShader("shaders/default.vert", "shaders/default.frag");
    Shader defaultShader("shaders/lighting.vert", "shaders/lighting.frag");
    defaultShader.use();
    defaultShader.setInt("material.diffuse", 0);
    defaultShader.setInt("material.specular", 1);

    // Images
    Image containerImage("resources/container2.png");
    Image containerSpecularImage("resources/container2_specular.png");
    Image faceImage("resources/awesomeface.png");

    // Textures
    Texture containerTex = Texture::fromImage(containerImage);
    Texture containerSpecularTex = Texture::fromImage(containerSpecularImage);
    Texture faceTex = Texture::fromImage(faceImage);

    // Materials
    auto containerMat = std::make_shared<Material>(containerTex, containerSpecularTex, 64.0f);
    auto lightIndicatorMat = std::make_shared<Material>(faceTex, containerSpecularTex, 64.0f);

    // Mesh
    std::shared_ptr<Mesh> cubeMesh(Mesh::createCubeDyn());
    cubeMesh->textures.push_back(containerTex);
    std::shared_ptr<Mesh> lightIndicatorMesh(Mesh::createCubeDyn());
    lightIndicatorMesh->textures.push_back(faceTex);

    // Scene
    scene = std::make_unique<Scene>();
    scene->addShader(defaultShader);

    // Root Node
    Node* rootNode = scene->getRootNode();

    // Camera
    Camera* camera = scene->createNode<Camera>();
    rootNode->addChild(camera);

    // Light
    LightNode* lightNode = scene->createNode<LightNode>();
    lightNode->setPosition(1.2f, 1.0f, 2.0f);
    lightNode->setScale(0.2f, 0.2f, 0.2f);
    lightNode->ambientColor = {0.2f, 0.2f, 0.2f};
    lightNode->diffuseColor = {0.5f, 0.5f, 0.5f};
    lightNode->specularColor = {1.0f, 1.0f, 1.0f};
    rootNode->addChild(lightNode);

    // Cube to show light location
    MeshNode* lightIndicator = scene->createNode<MeshNode>(lightIndicatorMesh, lightIndicatorMat, defaultShader);
    lightNode->addChild(lightIndicator);

    // Cubes
    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    for (auto pos : cubePositions) {
        MeshNode* cubeNode = scene->createNode<MeshNode>(cubeMesh, containerMat, defaultShader);
        cubeNode->setPosition(pos);
        rootNode->addChild(cubeNode);
    }

    // Nanosuit Model
    // Model nanosuitModel("resources/nanosuit/nanosuit.obj");

    // Program loop
    Uint32 frameTime;
    Uint32 lastFrameTime;

    while (!quit) {
        frameTime = SDL_GetTicks();

        processInput();

        Uint32 dt = frameTime - lastFrameTime;
        lastFrameTime = frameTime;
        update(dt / 1000.f);

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
    scene->update(dt);
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
