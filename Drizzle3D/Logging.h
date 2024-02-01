/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <string>
#include <iostream>
#include "base.h"

namespace Drizzle3D {
	class Drizzle3D_API Logging {
	public:
		Logging() {}

		void Error(std::string message, std::string who = "[Drizzle3D::Core] ");
		void Warning(std::string message, std::string who = "[Drizzle3D::Core] ");
		void Info(std::string message, std::string who = "[Drizzle3D::Core] ");
	};
}