#pragma once
#include <stdint.h>
#include <cstring>
namespace eg {

	// Non-owning raw buffer class
	struct Buffer
	{
		uint8_t* Data = nullptr;
		uint64_t Size = 0;

		Buffer() = default;
		// Might want to change the constructor to allocate memory once which is less safe
		Buffer(uint64_t size)
		{
			Allocate(size);
		}

		Buffer(const Buffer&) = default;

		static Buffer Copy(Buffer other)
		{
			Buffer buffer(other.Size);
			memcpy(buffer.Data, other.Data, other.Size);
			return buffer;
		}

		void Allocate(uint64_t size)
		{
			Release();
			Data = new uint8_t[size];
			Size = size;
		}

		void Release()
		{
			delete[] Data; 
			Data = nullptr; 
			Size = 0;
		}

		template<typename T>
		T* As()
		{
			return (T*)Data;
		}

		operator bool() const
		{
			return Data != nullptr;
		}

		bool operator==(const Buffer& other) const
		{
			return Data == other.Data && Size == other.Size;
		}
	};

	struct ScopedBuffer
	{
		ScopedBuffer() = default;
		ScopedBuffer(Buffer other): m_Buffer(other) {}

		ScopedBuffer(uint64_t size)
			: m_Buffer(size)
		{
		}

		~ScopedBuffer()
		{
			m_Buffer.Release();
		}

		uint8_t* Data() const
		{
			return m_Buffer.Data;
		}

		uint64_t Size() const
		{
			return m_Buffer.Size;
		}

		void Allocate(uint64_t size)
		{
			m_Buffer.Allocate(size);
		}

		template<typename T>
		T* As()
		{
			return (T*)m_Buffer.Data;
		}

		bool operator==(const ScopedBuffer& other) const
		{
			return m_Buffer == other.m_Buffer;
		}

		operator bool() const
		{
			return m_Buffer.Data != nullptr;
		}
	private:
		Buffer m_Buffer;
	};

}