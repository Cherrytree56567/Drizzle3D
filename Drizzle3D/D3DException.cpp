#include "D3DException.h"

namespace Drizzle3D {
	D3DException::D3DException(int line, const char* file) noexcept : line(line), file(file) {}

	const char* D3DException::what() const noexcept {
		std::ostringstream oss;
		oss << GetType() << std::endl << GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* D3DException::GetType() const noexcept {
		return "Drizzle3D Exception";
	}

	int D3DException::GetLine() const noexcept {
		return line;
	}

	const std::string& D3DException::GetFile() const noexcept {
		return file;
	}

	std::string D3DException::GetOriginString() const noexcept {
		std::ostringstream oss;
		oss << "[File] " << file << std::endl << "[Line] " << line;
		return oss.str();
	}
}