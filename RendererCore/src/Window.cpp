/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Window.h"
#include "WindowRegister.h"
#include "Debug.h"
#ifdef _DEBUG
#include <iostream>
#endif

namespace LCH
{
	Window::Window(int width, int height, std::optional<int> icon, const wchar_t* title)
		: width(width), height(height), icon(icon), hwnd(nullptr)
	{
		Initialize(width, height, title);
	}

	Window::~Window()
	{
		DestroyWindow(hwnd);
	}

	LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			// ��������һ����Ϊ�����ڣ����ٺ�Ҫ�˳��߳�
			PostQuitMessage(0);
			return 0;

			// ���°���
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			break;

			// �ɿ�����
		case WM_KEYUP:
		case WM_SYSKEYUP:
			break;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	std::optional<int> Window::ProcessMessage()
	{
		MSG msg = { 0 };

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return (int)msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return std::nullopt;
	}

	std::optional<int> Window::GetIcon() const noexcept
	{
		return icon;
	}

	void Window::Initialize(int width, int height, const wchar_t* title)
	{
		// ��ȡ����������
		WindowRegister* const windowRegister = WindowRegister::GetInstance();
		std::wstring wndClassName = windowRegister->GetWindowClassName(WindowType::Default);

		// �ͻ��������С
		RECT rect = { 0, 0, width, height };

		// ���ݿͻ������͸߼����������ڵĿ�͸�
		ASSERT(AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false), true);
		// ��������
		ASSERT(hwnd = CreateWindow(wndClassName.c_str(), title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
			nullptr, nullptr, windowRegister->GetHInstance(), this), true);

		ShowWindow(hwnd, SW_SHOW);
	}
}
