#pragma once
#ifndef EG_PLATFORM_LINUX
#include <xhash>
#else
#include <xxhash.h>
#endif

namespace eg {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};

}

namespace std {

	template<>
	struct hash<eg::UUID>
	{
		std::size_t operator()(const eg::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};

}

