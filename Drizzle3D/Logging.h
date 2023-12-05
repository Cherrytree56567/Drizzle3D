#pragma once
#include <string>
#include <iostream>

namespace Drizzle3D {
	class Logging {
	public:
		Logging() {}

		void Error(std::string message, std::string who = "[Drizzle3D::Core] ");
		void Warning(std::string message, std::string who = "[Drizzle3D::Core] ");
		void Info(std::string message, std::string who = "[Drizzle3D::Core] ");
	};
}