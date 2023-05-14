/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Keyboard.hpp"
#include "Diagnostics/Debug.hpp"

namespace TG::Input
{
	Keyboard::Keyboard() = default;

	Keyboard::~Keyboard() = default;

	void Keyboard::Update()
	{
		m_keyDown.reset();
		m_keyUp.reset();
	}

    void Keyboard::Receive(const Event &e)
    {
        auto key = static_cast<size_t>(e.key);
        switch (e.type)
        {
            case EventType::Press:
                // 第一次按下，key down为true
                if (!m_keyHold.test(key))
                    m_keyDown[key] = true;
                m_keyHold[key] = true;
                break;

            case EventType::Release:
                // 放开按键，key up为true(不存在第一次放开，每一次WM_KEYUP都是第一次放开)
                m_keyUp[key] = true;
                m_keyHold[key] = false;
                break;

            case EventType::Char:
            {
                if (e.data)
                {
                    auto data = static_cast<KeyboardData*>(e.data);
                    if (std::isprint(data->c))
                        Debug::LogLine(data->c);
                }
                break;
            }

            default:
                break;
        }

        if (m_spyKeyboard)
            Debug::LogLine(std::format(L"Key: {:<20} Event: {:<20} ", EventInfo::keysName[e.key], EventInfo::eventTypes[e.type]));
    }

    void Keyboard::SpyEvent(bool enable)
    {
        m_spyKeyboard = enable;
    }

    bool Keyboard::GetKey(KeyCode k)
    {
        if (!Contains(k))
            return false;

        auto pos = static_cast<size_t>(k);
        return m_keyHold.test(pos);
    }

    bool Keyboard::GetKeyDown(KeyCode k)
    {
        if (!Contains(k))
            return false;

        auto pos = static_cast<size_t>(k);
        return m_keyDown.test(pos);
    }

    bool Keyboard::GetKeyUp(KeyCode k)
    {
        if (!Contains(k))
            return false;

        auto pos = static_cast<size_t>(k);
        return m_keyUp.test(pos);
    }

	bool Keyboard::Contains(KeyCode k)
	{
		auto index = static_cast<unsigned char>(k);
		if (index > 0x08 && index <= 0xDE)
			return true;
		return false;
	}
}