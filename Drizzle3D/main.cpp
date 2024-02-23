/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include <GLAD/glad.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "App.h"

void Update(Drizzle3D::App* app) {

}

Drizzle3D_API int main() {

    /*
    * NOTE:
    * Key Released and Mouse Released
    */
    Drizzle3D::App app;

    app.update = Update;

    app.Run();

    return 0;
}