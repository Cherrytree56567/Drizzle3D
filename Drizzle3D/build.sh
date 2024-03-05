sudo apt install -y git gcc
git submodule init
git submodule update
gcc -Ivendor -Ivendor/Glad/include -Ivendor/GLFW/include -Ivendor/imgui -Ivendor/glm -Ivendor/Assimp/include -DDrizzle3D_LINUX -fPIC -std=c++20 -fpermissive  -shared -o x64/Debug/libDrizzle3D.so *.cpp
