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

#include "pch.h"

namespace LCH
{
	WindowClassRegister* WindowClassRegister::instance = nullptr;
	std::mutex WindowClassRegister::mutex;

	WindowClassRegister::WindowClassRegister()
	{
		hInstance = GetModuleHandle(nullptr);
		ASSERT(hInstance, true);

		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProcSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.hIconSm = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = L"Default";

		ASSERT(RegisterClassEx(&wc), true);
		windowClassMap[WindowType::Default] =  L"Default";
	}

	WindowClassRegister::~WindowClassRegister()
	{
		for (auto& pair : windowClassMap)
			UnregisterClass(pair.second.c_str(), hInstance);
	}

	LRESULT WindowClassRegister::WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
#ifdef _DEBUG
			SetLastError(0);
#endif
			auto offset = SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
#ifdef _DEBUG
			ASSERT(offset || !GetLastError(), true);
#endif
			offset = SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcThunk));
#ifdef _DEBUG
			ASSERT(offset || !GetLastError(), true);
#endif
			// 如果icon索引不为0，则更换icon图标
			if (int icon = pWnd->GetIcon())
			{
				offset = SetClassLongPtr(
					hwnd, GCLP_HICON,
					reinterpret_cast<LONG_PTR>(LoadIcon(pCreate->hInstance, MAKEINTRESOURCE(icon)))
				);
#ifdef _DEBUG
				ASSERT(offset || !GetLastError(), true);
#endif
			}
			return pWnd->WindowProc(hwnd, msg, wParam, lParam);
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	LRESULT WindowClassRegister::WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWnd->WindowProc(hwnd, msg, wParam, lParam);
	}

	WindowClassRegister* const WindowClassRegister::GetInstance()
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (instance == nullptr)
			instance = new WindowClassRegister();
		return instance;
	}

	Window::Window(int width, int height, int icon, const wchar_t* title)
		: width(width), height(height), icon(icon), hwnd(nullptr)
	{
		// 初始化窗口，使用虚函数，不同类型的窗口可以使用自己的初始化窗口函数
		Initialize(width, height, title);
	}

	Window::~Window()
	{
	}

	LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			// 基础窗口一般作为主窗口，销毁后要退出线程
			PostQuitMessage(0);
			return 0;
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

	inline int Window::GetIcon() const
	{
		return icon;
	}

	void Window::Initialize(int width, int height, const wchar_t* title)
	{
		// 获取窗口类名称
		WindowClassRegister* const windowRegister = WindowClassRegister::GetInstance();
		std::wstring wndClassName = windowRegister->windowClassMap[WindowType::Default];

		// 客户端区域大小
		RECT rect = { 0, 0, rect.left + width, rect.top + height };

		// 根据客户区域宽和高计算整个窗口的宽和高
		ASSERT(AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false), true);
		// 创建窗口
		ASSERT(hwnd = CreateWindow(wndClassName.c_str(), title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
			nullptr, nullptr, windowRegister->hInstance, this), true);

		ShowWindow(hwnd, SW_SHOW);
	}
}
