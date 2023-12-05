#pragma once
#include "Base.h"

namespace Drizzle3D {
	class D3DException : public std::exception {
	public:
		D3DException(int line, const char* file) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
		std::string GetOriginString() const noexcept;
	private:
		int line;
		std::string file;
	protected:
		mutable std::string whatBuffer;
	};
}