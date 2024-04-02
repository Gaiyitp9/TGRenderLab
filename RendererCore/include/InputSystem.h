/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <bitset>
#include <queue>
#include "KeyCode.h"
#include "InputEvent.h"

namespace LCH
{
	class InputSystem
	{
		friend class Window;
	public:
		InputSystem();
		InputSystem(const InputSystem&) = delete;
		InputSystem& operator=(const InputSystem&) = delete;
		~InputSystem();

		void Update();

		bool GetKey(KeyCode key) const;
		bool GetKeyDown(KeyCode key) const;
		bool GetKeyUp(KeyCode key) const;

	private:
		void OnKeyPressed(size_t key);
		void OnKeyReleased(size_t key);

	private:
		static constexpr unsigned int nKeys = 256u;
		std::bitset<nKeys> keyStates;		// ����״̬
		std::queue<InputEvent> events;		// �����¼�����
		std::queue<char> chars;				// �����ַ�����
	};
}