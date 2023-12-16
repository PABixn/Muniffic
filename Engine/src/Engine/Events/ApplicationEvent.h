#pragma once
#include "Event.h"
namespace eg {
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			:m_Width(width), m_Height(height){}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		};

		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(WindowResize);
	private:
		uint32_t m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent()
			 {}
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(WindowClose);
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowCloseEvnet";
			return ss.str();
		};
	};

	class ENGINE_API AppTickEvent : public Event
	{
		AppTickEvent()
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppTickEvent";
			return ss.str();
		};
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(AppTick);
	};

	class ENGINE_API AppUpdateEvent : public Event
	{
		AppUpdateEvent()
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppUpdateEvent";
			return ss.str();
		};
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(AppUpdate);
	};

	class ENGINE_API AppRenderEvent : public Event
	{
		AppRenderEvent()
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppRenderEvent";
			return ss.str();
		};
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
		EVENT_CLASS_TYPE(AppRender);
	};
}