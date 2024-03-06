sudo apt install g++
g++ -Ivendor -Ivendor/Drizzle3D -DDrizzle3D_LINUX -std=c++20 -fpermissive -g -shared -fPIC -L../Drizzle3D/x64/Debug -lDrizzle3D -o ExampleProject.elf *.cpp