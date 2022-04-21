/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <array>

namespace LCH::Math
{
	// 数学工具类
	class MathUtil
	{
	public:
		template <typename T> 
		inline static bool IsAligned(T value, size_t alignment)
		{
			return 0 == ((size_t)value & (alignment - 1));
		}
	};

	template<typename T, size_t N, size_t Alignment = 16>
	class alignas(Alignment) aligned_array : public std::array<T, N> {};
}