#pragma once
#include "Base.h"

namespace Drizzle3D {
	class Timer {
	public:
		Timer();
		float Mark();
		float Peek() const;
	private:
		std::chrono::steady_clock::time_point last;
	};
}