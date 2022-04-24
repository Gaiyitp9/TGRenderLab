/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Mouse.h"
#include "Diagnostics/Debug.h"

namespace LCH
{
	Mouse::Mouse()
	{

	}

	Mouse::~Mouse()
	{

	}

	void Mouse::Update()
	{
		mouseDown.reset();
		mouseUp.reset();
		while (!eventBuffer.empty())
		{
			InputEvent e = eventBuffer.front();
			eventBuffer.pop();

			// 监控鼠标
			if (spyMouse)
				SpyMouseEvent(e);

			size_t key = static_cast<size_t>(e.key);
			switch (e.type)
			{
			case InputEvent::Type::Press:
				mouseDown[key] = true;
				mouseStates[key] = true;
				break;

			case InputEvent::Type::Release:
				mouseUp[key] = true;
				mouseStates[key] = false;
				break;
			}
		}
	}

	void Mouse::OnMouseMove(const POINTS& position)
	{
		this->position = position;
		eventBuffer.push(InputEvent{ KeyCode::None, InputEvent::Type::MouseMove });
		TrimEventBuffer();
	}

	void Mouse::OnButtonPress(KeyCode key)
	{
		eventBuffer.push(InputEvent{ key, InputEvent::Type::Press });
		TrimEventBuffer();
	}

	void Mouse::OnButtonRelease(KeyCode key)
	{
		eventBuffer.push(InputEvent{ key, InputEvent::Type::Release });
		TrimEventBuffer();
	}

	void Mouse::OnWheelRoll(KeyCode key, short delta)
	{
		eventBuffer.push(InputEvent{ key, InputEvent::Type::WheelRoll });
		TrimEventBuffer();
		wheelDelta = delta;
	}

	const POINTS& Mouse::Position() const noexcept
	{
		return position;
	}

	short Mouse::RawWheelDelta() const noexcept
	{
		return wheelDelta;
	}

	short Mouse::WheelDelta() const noexcept
	{
		return wheelDelta / WHEEL_DELTA;
	}

	bool Mouse::IsMouseCode(KeyCode code)
	{
		unsigned char index = static_cast<unsigned char>(code);
		if (index >= 0x01 && index <= 0x04)
			return true;
		return false;
	}

	inline void Mouse::TrimEventBuffer()
	{
		while (eventBuffer.size() > BUFSIZE)
			eventBuffer.pop();
	}

	void Mouse::SpyMouseEvent(InputEvent e)
	{
		switch (e.type)
		{
		case InputEvent::Type::MouseMove:
			Debug::Log(std::format(L"{}\t", e));
			Debug::LogLine(std::format(L"MouseX: {}\tMouseY: {}", position.x, position.y));
			break;

		case InputEvent::Type::WheelRoll:
			Debug::Log(std::format(L"{}\t", e));
			Debug::LogLine(std::format(L"Raw wheel delta: {}\tWheel Delta: {}", RawWheelDelta(), WheelDelta()));
			break;

		default:
			Debug::LogLine(std::format(L"{}", e));
		}
	}
}