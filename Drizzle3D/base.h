#pragma once

#ifdef Drizzle3D_LINUX
#define Drizzle3D_API __attribute__((visibility("default")))
#else
#ifdef Drizzle3D_EXPORTS
#define Drizzle3D_API __declspec(dllexport)
#else
#define Drizzle3D_API __declspec(dllimport)
#endif
#endif
