#pragma once

#include <chrono>

namespace eg {

	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Reset();

		float Elapsed();

		float ElapsedMillis();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

}
