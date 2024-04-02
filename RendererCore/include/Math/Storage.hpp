/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <cassert>

namespace LCH::Math
{
	// 对齐数组
	template<typename T, int Size, int Alignment>
	struct PlainArray
	{
		alignas(Alignment) T array[Size];
	};

	// 非对齐数组
	template<typename T, int Size>
	struct PlainArray<T, Size, 0>
	{
		T array[Size];
	};

	// 分配对齐内存
	inline void* handmade_aligned_malloc(std::size_t size, std::size_t alignment)
	{
		assert(alignment >= sizeof(void*) && (alignment & (alignment - 1)) == 0 && "Alignment must be at least sizeof(void*) and a power of 2");

		void* original = std::malloc(size + alignment);
		if (original == nullptr) 
			return nullptr;
		void* aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(original) & ~(size_t(alignment - 1))) + alignment);
		*(reinterpret_cast<void**>(original) - 1) = original;
	}

	// 释放对齐内存
	inline void handmade_aligned_free(void* ptr)
	{
		if (ptr)
			std::free(*(reinterpret_cast<void**>(ptr) - 1));
	}

	// 根据条件分配动态内存(默认16字节对齐)
	template<int Alignment>
	inline void* conditional_aligned_alloc(size_t size)
	{
		return std::malloc(size);
	}

	template<>
	inline void* conditional_aligned_alloc<32>(size_t size)
	{
		return handmade_aligned_malloc(size, 32);
	}

	// 根据条件释放内存
	template<int Alignment>
	inline void conditional_aligned_free(void* ptr)
	{
		if (ptr)
			std::free(ptr);
	}

	template<>
	inline void conditional_aligned_free<32>(void* ptr)
	{
		handmade_aligned_free(ptr);
	}

	template<typename T, int Size, int Rows, int Cols, int Alignment> 
	class Storage
	{
	public:
		const T& operator[](size_t index) const { return m_data.array[index]; }
		T& operator[](size_t index) { return m_data.array[index]; }
		T const* data() const { return m_data.array; }
		int rows() { return Rows; }
		int cols() { return Cols; }
		int size() { return Size; }

	private:
		PlainArray<T, Size, Alignment> m_data;
	};

	template<typename T, int Alignment>
	class Storage<T, Dynamic, Dynamic, Dynamic, Alignment>
	{
	public:
		Storage() : m_data(nullptr), m_rows(0), m_cols(0) {}
		~Storage() { conditional_aligned_free<Alignment>(m_data); }

		void resize(int size, int rows, int cols)
		{
			if (size != m_rows * m_cols)
			{
				conditional_aligned_free<Alignment>(m_data);
				if (size > 0)
					m_data = static_cast<T*>(conditional_aligned_alloc<Alignment>(sizeof(T) * size));
				else
					m_data = nullptr;
			}
			m_rows = rows;
			m_cols = cols;
			m_size = size;
		}

		T const* data() const { return m_data; }
		int rows() { return m_rows; }
		int cols() { return m_cols; }
		int size() { return m_size; }

	private:
		T* m_data;
		int m_rows;
		int m_cols;
		int m_size;
	};
}