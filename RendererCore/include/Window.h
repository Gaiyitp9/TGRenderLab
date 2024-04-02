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
#include "resource.h"

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
		static WindowClassRegister* const GetInstance();

	private:
		WindowClassRegister();
		~WindowClassRegister();
		WindowClassRegister(const WindowClassRegister&) = delete;
		void operator=(const WindowClassRegister&) = delete;

		static LRESULT CALLBACK WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		// ��¼��ͬ���ʹ��ڵ�����
		std::unordered_map<WindowType, std::wstring> windowClassMap;
		// ʵ�����
		HINSTANCE hInstance;

	private:
		// ����ע��������
		static WindowClassRegister* instance;
		// �����������ڶ��̷߳��ʴ���ע��������
		static std::mutex mutex;
	};

	// ������
	class Window
	{
	public:
		Window(int width, int height, const wchar_t* title = L"IGGSZ Lab");
		Window(const Window&) = delete;
		void operator=(const Window&) = delete;
		~Window();

		virtual LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static std::optional<int> ProcessMessage();

	protected:
		virtual void Initialize(int width, int height, const wchar_t* title);

	private:
		HWND hwnd;
		int width, height;
	};
}