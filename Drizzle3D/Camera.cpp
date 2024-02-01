/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "RenderingLayer.h"

namespace Drizzle3D {
    void RenderingLayer::AddCamera(const char* id, Camera theCamera) {
        Camera a = theCamera;
        a.ID = (char*)id;
        Cameras.push_back(a);
    }

    void RenderingLayer::SwitchCamera(const char* id) {
        for (int i = 0; i < Cameras.size(); i++) {
            if (Cameras[i].ID == id) {
                current_camera = (char*)id;
            }
        }
    }

    Camera* RenderingLayer::returnCamera(const char* id) {
        for (int i = 0; i < Cameras.size(); i++) {
            if (Cameras[i].ID == id) {
                return &Cameras[i];
            }
        }
        return nullptr;
    }

    void RenderingLayer::RemoveCamera(const char* id) {
        for (int i = 0; i < Cameras.size(); i++) {
            if (Cameras[i].ID == id) {
                Cameras.erase(Cameras.begin() + i);
            }
        }
    }
    Camera RenderingLayer::ReturnActiveCamera() {
        for (int i = 0; i < Cameras.size(); i++) {
            if (Cameras[i].ID == current_camera) {
                return Cameras[i];
            }
        }
    }

    Camera RenderingLayer::GetCameraFromID(char* cam) {
        for (int i = 0; i < Cameras.size(); i++) {
            if (Cameras[i].ID == cam) {
                return Cameras[i];
            }
        }
    }
}