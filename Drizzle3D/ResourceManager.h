/*
***********************************************************************
*                                                                     *
* Drizzle3D © 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#pragma once
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <cstring>
#include "base.h"

namespace Drizzle3D {
	struct Resource {
		std::string content;
		const char mode[2];
	};

	class ResourceManager {
	public:
		Drizzle3D_API Resource loadFile(const std::string& filePath, const char mode[2]);

		Drizzle3D_API bool fileExists(const std::string& filePath) const;

		Drizzle3D_API void writeFile(const std::string& filePath, const std::string& content);

		Drizzle3D_API std::string& getTempFileContent(const std::string& filePath);
	private:
		std::unordered_map<std::string, std::string> resources;
	};
}