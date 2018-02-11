//
// Created by lasagnaphil on 2/8/18.
//

#ifndef GENGINE_INPUTMANAGER_H
#define GENGINE_INPUTMANAGER_H


#include <SDL2/SDL_events.h>
#include <unordered_map>

#include <glm/ext.hpp>

class InputManager {
public:
    static InputManager* getInstance() {
        if (!instance) {
            instance = new InputManager();
        }
        return instance;
    }

    void handleEvents(SDL_Event& ev) {
        switch (ev.type) {
            case SDL_KEYDOWN:
                keyMap[ev.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                keyMap[ev.key.keysym.sym] = false;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&mouseX, &mouseY);
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseMap[ev.button.button] = true;
                break;
            case SDL_MOUSEBUTTONUP:
                mouseMap[ev.button.button] = false;
                break;
            case SDL_MOUSEWHEEL:
                int wheelAmount = ev.wheel.y;
        }
    }

    bool keyboardPressed(SDL_Keycode keycode) {
        return keyMap[keycode];
    }

    bool mousePressed(Uint8 button) {
        return mouseMap[button];
    }

    glm::ivec2 mousePos() {
        return {mouseX, mouseY};
    }

private:
    InputManager() {}

    static InputManager* instance;
    std::unordered_map<SDL_Keycode, bool> keyMap;
    std::unordered_map<Uint8, bool> mouseMap;

    int mouseX, mouseY;
    int prevMouseX, prevMouseY;
};


#endif //GENGINE_INPUTMANAGER_H
