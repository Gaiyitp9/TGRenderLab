/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../SlimWindows.h"
#include <exception>
#include <string>
#include <vector>

namespace LCH
{

	class BaseException : public std::exception
	{
		struct StackFrame
		{
			unsigned int index;
			std::string file;
			std::string function;
			unsigned int line;
		};

	public:
		BaseException(std::string description);
		~BaseException();

		virtual char const* what() const override;
		virtual char const* GetType() const noexcept;

	protected:
		std::string whatBuffer;							// �쳣��Ϣ
		std::vector<StackFrame> stackFrameInfo;			// ջ֡��Ϣ

	private:
		void StackTrace();								// ׷��ջ֡��Ϣ

		HANDLE hProcess;								// ��ǰ���̾��
		constexpr static USHORT FramesToCapture = 100;	// ���׷��ջ֡��
		constexpr static UINT MaxNameLen = 255;			// ����������󳤶�
	};
}