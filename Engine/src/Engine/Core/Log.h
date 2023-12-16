#pragma once

#include "Engine/Core/Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace eg {
	class  Log
	{
	public:

		static void Init();
		
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { 
			return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternio)
{
	return os << glm::to_string(quaternio);
}

//Core log macros
#define EG_CORE_FATAL(...) ::eg::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define EG_CORE_ERROR(...) ::eg::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EG_CORE_WARN(...)  ::eg::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EG_CORE_INFO(...)  ::eg::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EG_CORE_TRACE(...) ::eg::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client log macros
#define EG_CLIENT_FATAL(...) ::eg::Log::GetClientLogger()->critical(__VA_ARGS__)
#define EG_CLIENT_ERROR(...) ::eg::Log::GetClientLogger()->error(__VA_ARGS__)
#define EG_CLIENT_WARN(...)  ::eg::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EG_CLIENT_INFO(...)  ::eg::Log::GetClientLogger()->info(__VA_ARGS__)
#define EG_CLIENT_TRACE(...) ::eg::Log::GetClientLogger()->trace(__VA_ARGS__)

#define EG_TRACE(...)         ::eg::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EG_INFO(...)          ::eg::Log::GetClientLogger()->info(__VA_ARGS__)
#define EG_WARN(...)          ::eg::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EG_ERROR(...)         ::eg::Log::GetClientLogger()->error(__VA_ARGS__)
#define EG_CRITICAL(...)      ::eg::Log::GetClientLogger()->critical(__VA_ARGS__)