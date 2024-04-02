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
		Window(int width, int height, std::optional<int> icon = std::nullopt, const wchar_t* title = L"TG RenderLab");
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();
		static std::optional<int> ProcessMessage();	// ��������Ϣ

	public:
		virtual LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);	// ��Ϣ�����������������д
		std::optional<int> GetIcon() const noexcept;

	protected:
		// ��ʼ�����ڣ����ര�ڿ�����д��ʼ�����ں���
		virtual void Initialize(int width, int height, const wchar_t* title);

	private:
		HWND hwnd;
		int width, height;
		std::optional<int> icon; // ����icon����
	};
}