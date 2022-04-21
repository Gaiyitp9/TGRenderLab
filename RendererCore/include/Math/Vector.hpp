/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <immintrin.h>
#include "MathUtil.h"

namespace LCH::Math
{
	template<typename T>
	struct simd_trait;

	template<>
	struct simd_trait<float>
	{
		static constexpr size_t Alignment = 16;
	};

	template<>
	struct simd_trait<int>
	{
		static constexpr size_t Alignment = 16;
	};

	template<>
	struct simd_trait<double>
	{
		static constexpr size_t Alignment = 32;
	};

	template <typename T, size_t N>
	class Vector
	{
		using simd = simd_trait<T>;
	public:
		public Vector();

	private:
		aligned_array<T, N, simd::Alignment> data;
	};
}