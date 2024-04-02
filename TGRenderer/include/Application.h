/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "TimeSystem.h"
#include "InputSystem.h"
#include <locale>
#include <iostream>

namespace LCH
{
	class Application
	{
	public:
		Application();
		~Application();
		Application(const Application&) = delete;
		const Application& operator=(const Application&) = delete;

		int Run();

	private:
		std::locale locale;		// ��������
		TimeSystem timer;		// ʱ��ϵͳ
		InputSystem input;		// ����ϵͳ
	};
}