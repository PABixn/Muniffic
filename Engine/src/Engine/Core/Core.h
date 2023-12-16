#pragma once

#include <memory>

#include "Engine/Core/PlatformDetection.h"

#ifdef EG_DEBUG
#if defined(EG_PLATFORM_WINDOWS)
#define EG_DEBUGBREAK() __debugbreak()
#elif defined(EG_PLATFORM_LINUX)
#include <signal.h>
#define EG_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define EG_ENABLE_ASSERTS
#else
#define EG_DEBUGBREAK()
#endif

#define EG_EXPAND_MACRO(x) x
#define EG_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define ENGINE_API

namespace eg {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "Log.h"
#include "Engine/Core/Assert.h"