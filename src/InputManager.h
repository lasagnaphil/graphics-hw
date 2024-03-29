//
// Created by lasagnaphil on 2/8/18.
//

#ifndef GENGINE_INPUTMANAGER_H
#define GENGINE_INPUTMANAGER_H


#include "SDL_events.h"
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
            case SDL_MOUSEBUTTONDOWN:
                mouseMap[ev.button.button] = true;
                break;
            case SDL_MOUSEBUTTONUP:
                mouseMap[ev.button.button] = false;
                break;
            case SDL_MOUSEWHEEL:
                wheelAmount += ev.wheel.y;
        }
    }

    bool keyboardPressed(SDL_Keycode keycode) {
        return keyMap[keycode];
    }

    bool mousePressed(Uint8 button) {
        return mouseMap[button];
    }

    bool mouseReleased(Uint8 button) {
        return !mouseMap[button];
    }

    glm::ivec2 mousePos() {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return {x, y};
    }

    glm::ivec2 relMousePos() {
        int x, y;
        SDL_GetRelativeMouseState(&x, &y);
        return {x, y};
    }

    glm::ivec2 relWheelPos() {
        glm::ivec2 wheelAmountTemp = wheelAmount;
        wheelAmount = {0, 0};
        return wheelAmountTemp;
    }

private:
    InputManager() {}

    static InputManager* instance;
    std::unordered_map<SDL_Keycode, bool> keyMap;
    std::unordered_map<Uint8, bool> mouseMap;

    glm::ivec2 wheelAmount;

};


#endif //GENGINE_INPUTMANAGER_H
