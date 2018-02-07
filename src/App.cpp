//
// Created by lasagnaphil on 2/6/18.
//

#include "App.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static const int SCREEN_WIDTH  = 800;
static const int SCREEN_HEIGHT = 600;

static void sdl_die(const char * message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    exit(2);
}

void App::start() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdl_die("Couldn't initialize SDL");
    atexit (SDL_Quit);
    SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Create the window
    window = SDL_CreateWindow(
            "OpenGL Test",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL
    );

    if (window == NULL) sdl_die("Couldn't set video mode");

    mainContext = SDL_GL_CreateContext(window);
    if (mainContext == NULL)
        sdl_die("Failed to create OpenGL context");

    // Check OpenGL properties
    printf("OpenGL loaded\n");
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    // Use v-sync
    SDL_GL_SetSwapInterval(1);

    int w,h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);

    // Compile default shaders
    triangleShader = std::make_unique<Shader>("shaders/triangle.vert", "shaders/multitexture_triangle.frag");

    // Create triangles
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Load textures
    stbi_set_flip_vertically_on_load(true);

    Image containerImage("resources/container.jpg");
    Image faceImage("resources/awesomeface.png");

    containerTex = std::make_unique<Texture>(
            containerImage.getData(), 0, GL_RGB, containerImage.getWidth(), containerImage.getHeight(),
            0, GL_RGB, GL_UNSIGNED_BYTE
    );
    containerTex->setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
    containerTex->setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
    containerTex->setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    containerTex->setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::cout << containerTex->getID() << std::endl;

    faceTex = std::make_unique<Texture>(
            faceImage.getData(), 0, GL_RGBA, faceImage.getWidth(), faceImage.getHeight(),
            0, GL_RGBA, GL_UNSIGNED_BYTE
    );
    faceTex->setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
    faceTex->setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
    faceTex->setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    faceTex->setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::cout << faceTex->getID() << std::endl;

    // set texture unit for each sampler
    triangleShader->use();
    triangleShader->setInt("texture1", 0);
    triangleShader->setInt("texture2", 1);

    // Program loop
    Uint32 frameTime;
    Uint32 lastFrameTime;

    while (!quit) {
        frameTime = SDL_GetTicks();

        processInput();

        Uint32 dt = frameTime - lastFrameTime;
        lastFrameTime = frameTime;
        update(dt);

        render();

        SDL_GL_SwapWindow(window);

        /*
        Uint32 totalFrameTime = SDL_GetTicks() - frameTime;
        if (totalFrameTime < msPerFrame) {
            SDL_Delay(msPerFrame - (SDL_GetTicks() - frameTime));
        }
         */
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
                case SDLK_q:
                    quit = true;
                    break;
            }
        }
    }
}

void App::update(float dt) {


}

void App::render() {
    // clear screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, containerTex->getID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, faceTex->getID());

    // render triangles
    triangleShader->use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

App::~App() {
}
