/****************************************************************
* Copyright (c) IGOTGAMES(IGG ShenZhen). All rights reserved.	*
*																*
* This code is licensed under the MIT License (MIT).			*
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF				*
* ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY			*
* IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR				*
* PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.				*
*																*
* Author: LiaoChenhan											*
*****************************************************************/

#pragma once

#include "pch.h"
#include <chrono>

namespace IGGSZLab
{
	class Timer
	{
	public:
		Timer();
		~Timer();

		float DeltaTime() const;
		float TotalTime() const;

		void Reset();
		void Pause();
		void Start();
		void Tick();

	private:
		float totalTime;
		float deltaTime;
		std::chrono::steady_clock::time_point last;
	};
}