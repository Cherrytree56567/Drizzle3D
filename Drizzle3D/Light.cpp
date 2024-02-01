/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "RenderingLayer.h"

namespace Drizzle3D {
    void RenderingLayer::AddLight(float id, Light theLight) {
        Light a = theLight;
        a.id = id;
        Lights.push_back(a);
    }

    Light* RenderingLayer::returnLight(float id) {
        for (int i = 0; i < Lights.size(); i++) {
            if (Lights[i].id == id) {
                return &Lights[i];
            }
        }
        return nullptr;
    }

    void RenderingLayer::RemoveLight(float id) {
        for (int i = 0; i < Lights.size(); i++) {
            if (Lights[i].id == id) {
                Lights.erase(Lights.begin() + i);
            }
        }
    }
}