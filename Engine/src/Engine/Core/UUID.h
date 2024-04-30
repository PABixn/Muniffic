#pragma once

namespace eg {

	class UUID
	{
	public:
		UUID();
		UUID(int64_t uuid);
		UUID(const UUID&) = default;

		operator int64_t() const { return m_UUID; }
	private:
		int64_t m_UUID;
	};

}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<eg::UUID>
	{
		std::size_t operator()(const eg::UUID& uuid) const
		{
			return (int64_t)uuid;
		}
	};

}

