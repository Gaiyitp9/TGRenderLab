/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "InputSystem.h"
#include <optional>

namespace LCH
{
	// ������
	class Window
	{
	public:
		Window(int width, int height, int icon = 0, const wchar_t* title = L"TG RenderLab");
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();

		virtual LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static std::optional<int> ProcessMessage();

	public:
		int GetIcon() const noexcept;
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