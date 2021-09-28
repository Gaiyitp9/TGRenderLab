/****************************************************************
* Copyright (c) LiaoChenhan. All rights reserved.				*
*																*
* This code is licensed under the MIT License (MIT).			*
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF				*
* ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY			*
* IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR				*
* PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.				*
*																*
* Author: LiaoChenhan											*
*****************************************************************/
#pragma once

#include <bitset>

namespace LCH
{
	enum class KeyCode : unsigned char
	{
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		J = 73,
	};

	class Keyboard
	{
	public:
		Keyboard();
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		~Keyboard();

		void OnKeyDown(unsigned char key);
		void OnKeyHoldDown(unsigned char key);
		void OnKeyUp(unsigned char key);

		bool GetKeyDown(KeyCode key);
		bool GetKeyHoldDown(KeyCode key);

	private:
		static constexpr unsigned int nKeys = 256u;
		std::bitset<nKeys> keyDown;
		std::bitset<nKeys> keyHoldDown;
	};
}