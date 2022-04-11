/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH
{
	class Math
	{
	public:
		template <typename T> 
		inline static bool IsAligned(T value, size_t alignment)
		{
			return 0 == ((size_t)value & (alignment - 1));
		}
	};
}