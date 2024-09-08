#include "Headers/Config.h"
#include <iostream>

namespace Config {
    Assets *Assets::assets = nullptr;

    Assets::Assets() {
        char buffer[MAX_PATH];

        GetModuleFileNameA(NULL, buffer, MAX_PATH);

        std::string path(buffer);
        path = path.substr(0, path.rfind('\\'));

        executablePath = path;

        font.loadFromFile(executablePath + "\\assets\\fonts\\roboto.ttf");
    }

    Assets &Assets::get() {
        if (assets == nullptr) {
            assets = new Assets();
        }

        return *assets;
    }
}