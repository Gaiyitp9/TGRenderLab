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

namespace IGGSZLab
{
	// ��������
	enum class WindowType
	{
		Default
	};

	// ����ע�����࣬����ע�ᴰ��
	class WindowClassRegister
	{
	public:
		// ����ע��������
		static const WindowClassRegister* GetInstance();

	protected:
		WindowClassRegister();
	private:
		WindowClassRegister(WindowClassRegister&) = delete;
		void operator=(const WindowClassRegister&) = delete;

	public:
		std::unordered_map<WindowType, std::string> windowClassMap;

	private:
		static WindowClassRegister* instance;
		static std::mutex mutex;
	};

	class Window
	{
	public:
		Window(HINSTANCE hInstance, float width, float height, WindowType type = WindowType::Default);
		~Window();

	public:
		HWND hwnd;
		HINSTANCE hInstance;
	};
}