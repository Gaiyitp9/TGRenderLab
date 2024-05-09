/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Window.h"
#include "Input/Event.h"
#include <functional>

namespace TG::PAL
{
	class MainWindow : public Window
	{
	public:
		MainWindow(int x, int y, int width, int height, wchar_t const* name = L"TG GraphicLab", HWND parent = nullptr);
		MainWindow(const MainWindow&) = delete;
        MainWindow(MainWindow&&) = delete;
		MainWindow& operator=(const MainWindow&) = delete;
        MainWindow& operator=(MainWindow&&) = delete;
		~MainWindow() override = default;

		// 窗口名称
        [[nodiscard]] const std::wstring& Name() const noexcept { return m_name; }
		// 设置窗口的icon
		void SetIcon(wchar_t const* iconPath) const;
		// 添加输入事件监听器
		void SetInputListener(const std::function<void(const Input::Event &)> &listener) noexcept
		{
			m_listener = listener;
		}
		// 设置窗口状态回调
        void SetStateCallback(const std::function<void()>& resume,
                              const std::function<void()>& suspend) noexcept
        {
        	m_resume = resume;
        	m_suspend = suspend;
        }

	private:
		LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) override;	                // 消息处理函数

		std::wstring m_name;				                    // 主窗口标题，使用wstring的目的是窗口自己保存一份

        std::function<void(const Input::Event&)> m_listener;    // 输入事件监听器
        std::function<void()> m_resume;                         // 窗口恢复回调
        std::function<void()> m_suspend;                        // 窗口暂停回调
	};
}