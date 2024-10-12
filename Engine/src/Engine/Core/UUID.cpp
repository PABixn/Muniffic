#include "egpch.h"
#include "UUID.h"

#include <random>


namespace eg{

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<int64_t> s_UniformDistribution;

	UUID::UUID()
		: m_UUID(s_UniformDistribution(s_Engine))
	{
		EG_PROFILE_FUNCTION();
	}

	UUID::UUID(int64_t uuid)
		: m_UUID(uuid)
	{
		EG_PROFILE_FUNCTION();
	}

}
