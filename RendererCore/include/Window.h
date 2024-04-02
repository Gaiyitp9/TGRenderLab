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
		static const WindowClassRegister* GetInstance();

	private:
		WindowClassRegister();
		WindowClassRegister(WindowClassRegister&) = delete;
		void operator=(const WindowClassRegister&) = delete;

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		std::unordered_map<WindowType, std::string> windowClassMap;

	private:
		// ����ע��������
		static WindowClassRegister* instance;
		// �����������ڶ��̷߳��ʴ���ע��������
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