/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Window.h"
#include "WindowRegister.h"
#include "Diagnostics/Debug.h"
#ifdef _DEBUG
#include <iostream>
#endif

namespace LCH
{
	Window::Window(int width, int height, wchar_t const* title, HWND parent)
		: width(width), height(height), hwnd(nullptr), name(title), parentHwnd(parent)
	{
		Initialize();
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

	const std::wstring& Window::GetName() const noexcept
	{
		return name;
	}

	const HWND Window::GetHwnd() const noexcept
	{
		return hwnd;
	}

	const HWND Window::GetParentHwnd() const noexcept
	{
		return parentHwnd;
	}

	void Window::Initialize()
	{
		// ��ȡ����������
		WindowRegister* const windowRegister = WindowRegister::GetInstance();
		const std::wstring& wndClassName = windowRegister->GetWindowClassName(WindowType::Default);

		// �ͻ��������С
		RECT rect = { 0, 0, width, height };

		// ���ݿͻ������͸߼����������ڵĿ�͸�
		ASSERT(AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false), true);
		// ��������
		ASSERT(hwnd = CreateWindow(wndClassName.c_str(), name.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
			parentHwnd, nullptr, windowRegister->GetHInstance(), this), true);

		ShowWindow(hwnd, SW_SHOW);
	}
}
