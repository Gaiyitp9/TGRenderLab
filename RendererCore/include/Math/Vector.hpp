/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Simd.hpp"
#include "MathUtil.h"

namespace LCH::Math
{
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