/****************************************************************
* Copyright (c) LiaoChenhan. All rights reserved.				*
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

#include <chrono>

namespace LCH
{
	class TimeSystem
	{
	public:
		TimeSystem();
		TimeSystem(const TimeSystem&) = delete;
		TimeSystem& operator=(const TimeSystem&) = delete;
		~TimeSystem();

		float DeltaTime() const;
		float TotalTime() const;
		std::wstring Time() const;

		void Reset();
		void Pause();
		void Start();
		void Tick();

	private:
		double deltaTime = 0.0;		// 单位毫秒(ms)
		bool stopped = false;

		std::chrono::system_clock wallClock;

		std::chrono::steady_clock timer;		
		std::chrono::steady_clock::time_point base;			// 开始运行的时间点
		std::chrono::steady_clock::time_point last;			// 上一帧的时间点
		std::chrono::steady_clock::time_point stop;			// 暂停的时间点
		std::chrono::duration<double, std::milli> paused;	// 已暂停的时间
	};
}