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
		double deltaTime = 0.0;		// ��λ����(ms)
		bool stopped = false;

		std::chrono::system_clock wallClock;

		std::chrono::steady_clock timer;		
		std::chrono::steady_clock::time_point base;			// ��ʼ���е�ʱ���
		std::chrono::steady_clock::time_point last;			// ��һ֡��ʱ���
		std::chrono::steady_clock::time_point stop;			// ��ͣ��ʱ���
		std::chrono::duration<double, std::milli> paused;	// ����ͣ��ʱ��
	};
}