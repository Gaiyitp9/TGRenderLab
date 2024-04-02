/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "InputSystem.h"

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
		void ConnectInputSystem(InputSystem* input);

	protected:
		virtual void Initialize(int width, int height, const wchar_t* title);

	private:
		HWND hwnd;
		int width, height;
		int icon;			// ����icon��������resource.h�ļ��ж���

		InputSystem* input;	// ����ϵͳ
	};
}