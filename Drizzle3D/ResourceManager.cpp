/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "ResourceManager.h"

namespace Drizzle3D {
    Resource ResourceManager::loadFile(const std::string& filePath, const char mode[2]) {
        auto it = resources.find(filePath);
        if (it != resources.end()) {
            return { it->second, (const char)mode};
        }
        else {
            std::ifstream file;
            std::string content;

            if (mode[0] == 'r') {
                file.open(filePath);
                if (!file.is_open()) {
                    std::cerr << "Failed to open file for reading: " << filePath << std::endl;
                    return Resource{ "", "" };
                }
                content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                file.close();
            }
            else {
                if (mode[1] == 'w') { // Read&Write
                    file.open(filePath, std::ios::in | std::ios::out);
                    if (!file.is_open()) {
                        std::cerr << "Failed to open file for reading and writing: " << filePath << std::endl;
                        return Resource{ "", "" };
                    }
                    content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                }
            }

            resources[filePath] = content;
            return { content, (const char)mode };
        }
    }

    bool ResourceManager::fileExists(const std::string& filePath) const {
        return resources.find(filePath) != resources.end();
    }

    void ResourceManager::writeFile(const std::string& filePath, const std::string& content) {
        auto it = resources.find(filePath);
        if (it != resources.end() && it->second.find("rw") != std::string::npos) {
            std::ofstream file(filePath);
            if (!file.is_open()) {
                std::cerr << "Failed to open file for writing: " << filePath << std::endl;
                return;
            }
            file << content;
            file.close();
        }
        else {
            std::cerr << "File not opened for writing or not found: " << filePath << std::endl;
        }
    }

    std::string& ResourceManager::getTempFileContent(const std::string& filePath) {
        auto it = resources.find(filePath);
        if (it != resources.end() && it->second.find("t") != std::string::npos) {
            return it->second;
        }
        else {
            std::cerr << "File not opened as temporary or not found: " << filePath << std::endl;
            static std::string emptyString;
            return emptyString;
        }
    }
}