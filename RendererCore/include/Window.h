/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "SlimWindows.h"
#include "InputSystem.h"
#include <optional>

namespace LCH
{
	// ������
	class Window
	{
	public:
		Window(int width, int height, const wchar_t* title = L"TG RenderLab", HWND parent = nullptr);
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		virtual ~Window();

		static std::optional<int> ProcessMessage();							// �������д��ڵ���Ϣ

		const HWND GetHwnd() const noexcept;
		const HWND GetParentHwnd() const noexcept;
		const std::wstring& GetName() const noexcept;

	private:
		virtual void Initialize();											// ��ʼ�����ڣ����ര�ڿ�����д��ʼ�����ں���
		virtual LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);	// ��Ϣ�����������������д

	private:
		HWND hwnd, parentHwnd;
		int width, height;
		std::wstring name;

		friend class WindowRegister;
	};
}