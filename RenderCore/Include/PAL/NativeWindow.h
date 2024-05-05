/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#ifdef _WIN64
#include "Windows/WindowCore.h"
#endif

namespace TG::PAL
{
    // 不同平台下的原生窗口
    class NativeWindow
    {
    public:
        NativeWindow(int x, int y, int width, int height, wchar_t const* title);

#ifdef _WIN64
    private:
        HWND m_hwnd;
#endif
    };

    // 轮询输入事件
    void PollEvents();
}
