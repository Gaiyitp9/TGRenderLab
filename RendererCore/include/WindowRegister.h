/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "SlimWindows.h"
#include <unordered_map>
#include <mutex>

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
		void Initialize(const std::vector<int>& icons);					// ��ʼ��ע������ע�����д�����
		HINSTANCE GetHInstance() const noexcept;
		const std::wstring& GetWindowClassName(const WindowType& type) const;
		std::wstring GetWindowMesssageInfo(const std::wstring& window, UINT msg, WPARAM wp, LPARAM lp) const;

	private:
		WindowRegister();
		~WindowRegister();
		WindowRegister(const WindowRegister&) = delete;
		const WindowRegister& operator=(const WindowRegister&) = delete;

		static LRESULT CALLBACK WindowProcSetup(HWND, UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK WindowProcThunk(HWND, UINT, WPARAM, LPARAM);

	private:
		static WindowRegister* instance;								// ����ע��������
		static std::mutex mutex;										// �����������ڶ��̷߳��ʴ���ע��������

		HINSTANCE hInstance;											// �����ļ�(.exe)���
		std::unordered_map<WindowType, std::wstring> windowClassName;	// ��¼��ͬ���ʹ��ڵ�����

		std::unordered_map<DWORD, std::wstring> windowMessage;			// ������Ϣ
	};
}