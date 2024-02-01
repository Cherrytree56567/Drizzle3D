/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "RenderingLayer.h"

namespace Drizzle3D {
    void RenderingLayer::AddObject(const char* name, Object theObject) {
        theObject.name = (char*)name;
        Objects.push_back(theObject);
    }

    Object* RenderingLayer::returnObject(const char* name) {
        for (int i = 0; i < Objects.size(); i++) {
            if (Objects[i].name == name) {
                return &Objects[i];
            }
        }
        return nullptr;
    }

    void RenderingLayer::RemoveObject(const char* name) {
        for (int i = 0; i < Objects.size(); i++) {
            if (Objects[i].name == name) {
                Objects.erase(Objects.begin() + i);
            }
        }
    }
}