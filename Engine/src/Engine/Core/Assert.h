#pragma once

//#include "Engine/Core/Core.h"
//#include "Engine/Core/Log.h"
#include <filesystem>

#ifdef EG_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define EG_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { EG##type##ERROR(msg, __VA_ARGS__); EG_DEBUGBREAK(); } }
#define EG_INTERNAL_ASSERT_WITH_MSG(type, check, ...) EG_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define EG_INTERNAL_ASSERT_NO_MSG(type, check) EG_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", EG_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define EG_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define EG_INTERNAL_ASSERT_GET_MACRO(...) EG_EXPAND_MACRO( EG_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, EG_INTERNAL_ASSERT_WITH_MSG, EG_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define EG_ASSERT(...) EG_EXPAND_MACRO( EG_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define EG_CORE_ASSERT(...) EG_EXPAND_MACRO( EG_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define EG_ASSERT(...)
#define EG_CORE_ASSERT(...)
#endif