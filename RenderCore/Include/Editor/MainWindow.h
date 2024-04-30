/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Window.h"

namespace TG
{
    class MainWindow final : public Window
    {
    public:
        MainWindow(int x, int y, int width, int height, wchar_t const* name = L"TG GraphicLab", const Window& parent);
        MainWindow(const MainWindow&) = delete;
        MainWindow(MainWindow&&) = delete;
        MainWindow& operator=(const MainWindow&) = delete;
        MainWindow& operator=(MainWindow&&) = delete;
        ~MainWindow() override = default;
    };
}
