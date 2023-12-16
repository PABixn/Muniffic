#pragma once
#include <cstdint>
#include <xhash>
#include <string>

namespace eg {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& other);

		operator uint64_t() const { return m_UUID; }
		operator uint64_t&() { return m_UUID; }

		//bool operator==(const UUID& other) const;
		//bool operator!=(const UUID& other) const;
		//
		//bool operator<(const UUID& other) const;
		//bool operator>(const UUID& other) const;
		//
		//bool operator<=(const UUID& other) const;
		//bool operator>=(const UUID& other) const;
		//
		//bool operator==(const uint64_t& other) const;
		//bool operator!=(const uint64_t& other) const;
		//
		//bool operator<(const uint64_t& other) const;
		//bool operator>(const uint64_t& other) const;
		//
		//bool operator<=(const uint64_t& other) const;
		//bool operator>=(const uint64_t& other) const;
		//
		//static UUID Create();
		//static UUID CreateFromString(const std::string& string);
		//static UUID CreateFromBytes(const uint8_t* bytes);
		//
		//std::string ToString() const;
		//const uint8_t* ToBytes() const;
		//
		//static constexpr size_t GetSize() { return 8; }
	private:
		uint64_t m_UUID;
	};

}

