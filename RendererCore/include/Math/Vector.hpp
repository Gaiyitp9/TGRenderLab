/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <immintrin.h>
#include <array>

namespace LCH::Math
{
	template <typename T, size_t N>
	struct Vector
	{
	public:

	private:
		std::array<T, N> data;
	};
}