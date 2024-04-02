/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "pch.h"

namespace LCH
{
	// ��������
	enum class WindowType
	{
		Default
	};

	// ����ע�����࣬����ע�ᴰ�ں���Ϣ
	class WindowRegister
	{
	public:
		static WindowRegister* const GetInstance();
		HINSTANCE GetHInstance() const noexcept;
		std::wstring GetWindowClassName(const WindowType&) const;
		std::wstring GetWindowMesssageInfo(DWORD, LPARAM, WPARAM) const;

	private:
		WindowRegister();
		~WindowRegister();
		WindowRegister(const WindowRegister&) = delete;
		WindowRegister& operator=(const WindowRegister&) = delete;

		static LRESULT CALLBACK WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		static WindowRegister* instance;								// ����ע��������
		static std::mutex mutex;										// �����������ڶ��̷߳��ʴ���ע��������

		HINSTANCE hInstance;											// �����ļ�(.exe)���
		std::unordered_map<WindowType, std::wstring> windowClassName;	// ��¼��ͬ���ʹ��ڵ�����
		std::unordered_map<DWORD, std::wstring> windowMessage;			// ������Ϣ����
	};
}