#pragma once
#include <map>
#include <string>
#include <any>
#include "base.h"
#include "Logging.h"

namespace Drizzle3D {
	class Flags {
	public:
		Drizzle3D_API Flags() {}

		Drizzle3D_API void AddFlag(const std::string& name, bool& pointer);

        Drizzle3D_API void ChangeFlag(const std::string& name, const bool newValue);

		Drizzle3D_API bool GetFlag(const std::string& name);

	private:
		std::map<std::string, bool*> flagMap;
		Logging log;
	};
}