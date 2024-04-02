/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "SlimWindows.h"
#include "InputSystem.h"
#include <optional>
#include <tchar.h>

namespace LCH
{
	// ������
	class Window
	{
	public:
		Window(int width, int height, PCTSTR title = L"TG RenderLab", HWND parent = nullptr);
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();
		static std::optional<int> ProcessMessage();	// ��������Ϣ

	public:
		virtual LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);	// ��Ϣ�����������������д
		const HWND GetHwnd() const noexcept;
		const HWND GetParentHwnd() const noexcept;
		const std::wstring& GetName() const noexcept;

	protected:
		// ��ʼ�����ڣ����ര�ڿ�����д��ʼ�����ں���
		virtual void Initialize();

	private:
		HWND hwnd, parentHwnd;
		int width, height;
		std::wstring name;
	};
}