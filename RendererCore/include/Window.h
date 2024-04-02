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

#include "Keyboard.h"
#include "Mouse.h"

namespace LCH
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
		WindowClassRegister& operator=(const WindowClassRegister&) = delete;

		static LRESULT CALLBACK WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		std::unordered_map<WindowType, std::wstring> windowClassMap;	// ��¼��ͬ���ʹ��ڵ�����
		HINSTANCE hInstance;											// ʵ�����

	private:
		static WindowClassRegister* instance;	// ����ע��������
		static std::mutex mutex;				// �����������ڶ��̷߳��ʴ���ע��������
	};

	// ������
	class Window
	{
	public:
		Window(int width, int height, int icon = 0, const wchar_t* title = L"IGGSZ TA Group");
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();

		virtual LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static std::optional<int> ProcessMessage();

	public:
		int GetIcon() const;

	protected:
		virtual void Initialize(int width, int height, const wchar_t* title);

	private:
		HWND hwnd;
		int width, height;
		int icon;			// ����icon��������resource.h�ļ��ж���

		Keyboard keyboard;
		Mouse mouse;
	};
}