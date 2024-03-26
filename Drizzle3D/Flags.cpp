#include "Flags.h"

namespace Drizzle3D {
	void Flags::AddFlag(const std::string& name, bool& pointer) {
		flagMap[name] = &pointer;
	}

    void Flags::ChangeFlag(const std::string& name, const bool newValue) {
        if (flagMap.find(name) != flagMap.end()) {
            try {
                *flagMap[name] = newValue;
            }
            catch (const std::bad_any_cast& e) {
                log.Error("Failed to cast to the correct type: " + (std::string)e.what() + "\n");
            }
        }
        else {
            log.Error("Flag with name " + name + " not found.\n");
        }
    }

    bool Flags::GetFlag(const std::string& name) {
        if (flagMap.find(name) != flagMap.end()) {
            return *flagMap[name];
        }
        else {
            log.Error("Flag with name " + name + " not found.\n");
            return false; 
        }
    }
}