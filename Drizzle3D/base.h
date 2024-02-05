#pragma once


#ifdef Drizzle3D_EXPORTS
#define Drizzle3D_API __declspec(dllexport)
#else
#define Drizzle3D_API __declspec(dllimport)
#endif
